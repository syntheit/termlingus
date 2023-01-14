#include <cstdlib>
#include <iostream>
#include "command.h"
#include "ThreadSafeQueue.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <filesystem>

#include <chrono>
#include <thread>

#include <sstream>
#include <iomanip>

int getFrameCount(const std::string& filename) {
	std::string output = raymii::Command::exec("ffprobe -v error -count_frames -select_streams v:0 -show_entries \
		stream=nb_read_frames -of default=nokey=1:noprint_wrappers=1 " + filename).output;
	return std::stoi(output);
}

int getFrameRate(const std::string& filename) {
	std::string output = raymii::Command::exec("ffprobe -v error -select_streams v:0 -show_entries \
		stream=r_frame_rate -of csv=s=x:p=0 " + filename).output;
	std::string token = output.substr(0, output.find("/"));
	return std::stoi(token);
}

ThreadSafeQueue<std::string> queue;
int frameCount = 0, frameRate = 0;

void displayImages() {
	int frameNum = 0;
	const double frameTime = 1000.0 / frameRate;

	const long startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long currentTime = startTime;

	while (frameNum < frameCount) {
		std::cout << queue.front() << std::endl;
		queue.pop();

		std::cout << std::endl << std::endl;
		std::cout << "Frame " << frameNum << " of " << frameCount << std::endl;
		std::cout << "Queue Size: " << queue.size() << std::endl;
		std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - currentTime << "ms" << std::endl;
		std::cout << "Current Time: " << currentTime - startTime << std::endl;
		std::cout << "Goal Time: " << (frameNum * frameTime) << std::endl;

		frameNum++;
		
		while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startTime < (frameNum * frameTime)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}

void renderNow(const std::string& filename, const int first, const int last) {
	raymii::Command::exec("ffmpeg -i " + filename + " -vf select='between(n\\," + std::to_string(first) + "\\," + std::to_string(last) + 
		"),setpts=PTS-STARTPTS' -vsync 0 images/out%07d.jpg -hide_banner -loglevel error");

	std::stringstream ss;

	for (int i = first; i < last; i++) {
		ss.str(std::string()); ss.clear();
		ss << std::setw(7) << std::setfill('0') << i - first + 1;
		std::string s = ss.str();

		std::string output = raymii::Command::exec("jp2a images/out" + s + ".jpg --color --fill --clear").output;
		queue.push(std::move(output));
		raymii::Command::exec("rm images/out" + s + ".jpg");
	}
}

void renderImages(const std::string& filename) {
	int frameNum = 0;

	while (frameNum < frameCount) {
		renderNow(filename, frameNum, frameNum + 250);
		frameNum += 250;

		while (queue.size() > 100) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

int main(int argc, char** argv) {
	frameCount = getFrameCount("video.mp4");
	frameRate = getFrameRate("video.mp4");

	// Start the rendering thread
	std::thread t1(renderImages, "video.mp4");

	// Preload the queue
	while (queue.size() < 50) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	displayImages();
	t1.join();
}
