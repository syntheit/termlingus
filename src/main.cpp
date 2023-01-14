#include "ThreadSafeQueue.hpp"
#include "command.h"
#include "ytdl.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

double round(double d) { return std::ceil(d * 100.0) / 100.0; }

int getFrameCount(const std::string &filename) {
  std::string output =
      raymii::Command::exec(
          "ffprobe -v error -count_frames -select_streams v:0 -show_entries \
		stream=nb_read_frames -of default=nokey=1:noprint_wrappers=1 " +
          filename)
          .output;
  return std::stoi(output);
}

int getFrameRate(const std::string &filename) {
  std::string output = raymii::Command::exec(
                           "ffprobe -v error -select_streams v:0 -show_entries \
		stream=r_frame_rate -of csv=s=x:p=0 " +
                           filename)
                           .output;
  std::string token = output.substr(0, output.find("/"));
  return std::stoi(token);
}

ThreadSafeQueue<std::string> queue;
int totalFrameCount = 0, frameRate = 0;
double secondsElapsed = 0, videoLength = 0;
std::string video_name = "video.mp4";

void displayImages() {
  int frameNum = 0;
  const double frameTime = 1000.0 / frameRate;

  const long startTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
  long currentTime = startTime;

  while (frameNum < totalFrameCount) {
    std::cout << queue.front() << std::endl;
    queue.pop();

    std::cout << std::endl << std::endl;
    std::cout << "Frame " << frameNum << " of " << totalFrameCount << std::endl;
    std::cout << "Queue Size: " << queue.size() << std::endl;
    std::cout << "Time Elapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                         .count() -
                     currentTime
              << "ms" << std::endl;
    std::cout << "Current Time: " << currentTime - startTime << std::endl;
    std::cout << "Goal Time: " << (frameNum * frameTime) << std::endl;
    std::cout << "Timestamp: " << round(secondsElapsed) << "s" << std::endl;
    std::cout << "Completion %: "
              << round(secondsElapsed / (totalFrameCount / frameRate) * 100)
              << "%" << std::endl;
    secondsElapsed = double(frameNum) / double(frameRate);

    frameNum++;

    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
                   .count() -
               startTime <
           (frameNum * frameTime)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
  }
}

void renderNow(const std::string &filename, const int first, const int last) {
  raymii::Command::exec("ffmpeg -i " + filename + " -vf select='between(n\\," +
                        std::to_string(first) + "\\," + std::to_string(last) +
                        "),setpts=PTS-STARTPTS' -vsync 0 images/out%07d.jpg "
                        "-hide_banner -loglevel error");

  std::stringstream ss;

  for (int i = first; i < last; i++) {
    ss.str(std::string());
    ss.clear();
    ss << std::setw(7) << std::setfill('0') << i - first + 1;
    std::string s = ss.str();

    std::string output = raymii::Command::exec("jp2a images/out" + s +
                                               ".jpg --color --fill --clear")
                             .output;
    queue.push(std::move(output));
    raymii::Command::exec("rm images/out" + s + ".jpg");
  }
}

void renderImages(const std::string &filename) {
  int frameNum = 0;

  while (frameNum < totalFrameCount) {
    renderNow(filename, frameNum, frameNum + 250);
    frameNum += 250;

    while (queue.size() > 100) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}

int main(int argc, char **argv) {
  std::map<std::string, std::string> flags;
  std::vector<std::string> allArgs(argv, argv + argc);
  for (unsigned int i = 0; i < allArgs.size(); ++i) {
    std::string cur = allArgs.at(i);
    // check to make sure the flag and commands are valid
    if (cur[0] == '-')
      flags.insert(std::pair<std::string, std::string>(
          cur.substr(1, cur.size()),
          i + 1 == allArgs.size() ? "" : allArgs.at(i + 1)));
  }
  std::string url = allArgs.size() > 1 ? allArgs.at(1) : "";

  // check to make sure it's a youtube link
  if (url != "") {
    std::cout << "the URL is: " << url << std::endl;
    load_video(url);
  }

  totalFrameCount = getFrameCount(video_name);
  frameRate = getFrameRate(video_name);

  // Start the rendering thread
  std::thread t1(renderImages, video_name);

  std::thread audio_thread(raymii::Command::exec, "play stream.mp3");

  // Preload the queue
  while (queue.size() < 50) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  displayImages();
  t1.join();
}
