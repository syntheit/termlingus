#include "bmp.h"
#include "client.h"
#include "command.h"
#include "gui.h"
#include "server.h"
#include "ytdl.h"
#include <algorithm>
#include <atomic>
#include <cassert>
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

#define PORT 2947

double round(double d) { return std::ceil(d * 100.0) / 100.0; }

int totalFrameCount = 0, frameRate = 0;
double secondsElapsed = 0, videoLength = 0;
std::string video_name = "video.mp4";

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
  int slash = output.find("/");
  std::string first = output.substr(0, slash);
  std::string second = output.substr(slash + 1, output.length());
  int result =
      slash == -1 ? std::stoi(first) : (std::stoi(first) / std::stoi(second));
  return result;
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
  if (url != "")
    video_name = load_video(url);

  totalFrameCount = getFrameCount("video.mp4");
  frameRate = getFrameRate("video.mp4");

  if (flags.count("s") == 1) {
    server_init();
  }

  if (flags.count("w") == 1) {
    std::string server_ip = flags.at("w");
    client_init(server_ip);
  }

  std::thread audio_thread(raymii::Command::exec, "play stream.mp3");

  // Start the rendering thread
  init_gui(video_name, totalFrameCount, frameRate);
}
