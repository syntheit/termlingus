#include <algorithm> // for count
#include <array>     // for array
#include <cassert>   // for assert
#include <cmath>     // for abs
#include <cstddef>   // for size_t
#include <cstdio>
#include <memory>    // for shared_ptr
#include <random>    // for random_device, mt19937
#include <string>    // for to_string, string
#include <utility>   // for move, swap
#include <sstream>

#include "command.h"

        // for CatchEvent, Renderer
#include "ftxui/component/event.hpp"              // for Event
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, vbox, window, Element, Elements
#include "ftxui/screen/screen.hpp"  // for Screen

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

#include "video.hpp"
#include "thumbnail.h"

using namespace ftxui;

int main() {
	std::string vidData = raymii::Command::exec("export PYTHONPATH=../py; python3 -c \"import search; " + 
    "print(search.most_popular_videos(" + std::to_string(10) + "))\"").output;
	std::cout << vidData << std::endl;

  // String stream to process vidData
  std::stringstream ss;
  ss.str(vidData);

  std::vector<Video> videoObjs;

  // Create a video object for each line
  while (ss.good()) {
    std::string id, title, channelTitle;

    std::getline(ss, id);
    std::getline(ss, title);
    std::getline(ss, channelTitle);


    if (id == "" || title == "" || channelTitle == "")
    { break; }
    videoObjs.push_back(Video(title, channelTitle, id));
  }

  std::string searchTxt = "";
  Component searchInput = Input(&searchTxt, "Enter a query");
  auto screen = ScreenInteractive::Fullscreen();
  int selectedItem = -1;

  auto component = Renderer([&selectedItem, videoObjs, searchInput] {
    Elements videos;

    std::string title = "", channelTitle = "";

    for (int i = 0; i < videoObjs.size(); i++) {
      videos.push_back(videoObjs[i].create(i == selectedItem));

      if (i == selectedItem) {
        title = videoObjs[i].title;
        channelTitle = videoObjs[i].channelTitle;
      }
    }

    return window(text("TermLingus"), vbox({
      separator(),
        hbox(text("Search: "), searchInput->Render()),
        separator(),
        text(title),
        text(channelTitle),
        separator(),
        flexbox(videos)
    }));
  });

  auto catch_event = CatchEvent(component, [&](Event event) {
    if (event == Event::Escape || event.input() == "q") {
      screen.ExitLoopClosure()();
    } else if (event == Event::ArrowLeft) {
      selectedItem = std::max(-1, selectedItem - 1);
    } else if (event == Event::ArrowRight) {
      selectedItem = std::min((int) videoObjs.size() - 1, selectedItem + 1);
    } else if (event == Event::Return) {
      if (selectedItem != -1) {
        // Search
      } else {
        // Go to video page
      }
    }

    return false;
  });

 screen.Loop(catch_event);
}
