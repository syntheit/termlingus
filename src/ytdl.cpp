//
// Created by bertoc2 on 1/14/2023.
//

#include "ytdl.h"
#include "command.h"
#include <iostream>

std::string load_video(const std::string& url) {
    raymii::CommandResult result = raymii::Command::exec(std::string("yt-dlp -S +size,+br,+res,+fps --print filename --no-simulate ") + url);

    return result.exitstatus ? "" : result.output.substr(0, result.output.size() - 1);
}
