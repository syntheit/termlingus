//
// Created by bertoc2 on 1/14/2023.
//

#include "ytdl.h"
#include "command.h"
#include <iostream>

std::string load_video(const std::string &url) {
  raymii::CommandResult result = raymii::Command::exec(
      std::string("yt-dlp -S +size,+br,+res,+fps --print filename "
                  "--no-simulate -f mp4 -o video.mp4 ") +
      url);
  raymii::CommandResult audio_result = raymii::Command::exec(
      std::string("ffmpeg -i video.mp4 -q:a 0 -map a stream.mp3"));

  return result.exitstatus ? ""
                           : result.output.substr(0, result.output.size() - 1);
}