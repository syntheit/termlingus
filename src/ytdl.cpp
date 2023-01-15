//
// Created by bertoc2 on 1/14/2023.
//

#include "ytdl.h"
#include "command.h"
#include <iostream>
#include <cassert>

std::string load_video(const std::string &url) {
  raymii::CommandResult result = raymii::Command::exec(
      std::string("yt-dlp -S +size,+br,+res,+fps --print id "
                  "--no-simulate -f mp4 -o video.mp4 ") +
      url);

  std::string id = result.output.substr(0, result.output.find('\n'));


  raymii::CommandResult video_result = raymii::Command::exec(
      std::string("ffmpeg -i video.mp4 -vf scale=200:-1 -pix_fmt bgr24 \"images/")
      + id + std::string("%03d.bmp\""));

  raymii::CommandResult audio_result = raymii::Command::exec(
      std::string("ffmpeg -i video.mp4 -q:a 0 -map a stream.mp3"));

  return result.exitstatus ? ""
                           : id;
}