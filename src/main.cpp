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
    video_name = load_video(url);
  }

  if (flags.count("s") == 1) {
    server_init();
  }

  if (flags.count("w") == 1) {
    std::string server_ip = flags.at("w");
    client_init(server_ip);
  }

  // Start the rendering thread
  init_gui(video_name, totalFrameCount, frameRate);

  std::thread audio_thread(raymii::Command::exec, "play stream.mp3");
}
