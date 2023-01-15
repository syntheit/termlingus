#include <cassert>
#include <cmath> // for sin, cos
#include <cstring>
#include <fstream>
#include <memory> // for allocator, shared_ptr, __shared_ptr_access
#include <stdint.h>
#include <string>  // for string, basic_string
#include <utility> // for move
#include <vector>  // for vector, __alloc_traits<>::value_type

#include "ftxui/component/component.hpp" // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/event.hpp"              // for Event
#include "ftxui/component/mouse.hpp"              // for Mouse
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"                   // for Canvas
#include "ftxui/dom/elements.hpp" // for canvas, Element, separator, hbox, operator|, border
#include "ftxui/screen/color.hpp" // for Color, Color::Red, Color::Blue, Color::Green, ftxui
#include "ftxui/screen/screen.hpp" // for Pixel

#include "bmp.h"
#include "thumbnail.h"

int init_gui(std::string video_id, uint64_t frame_count, uint16_t framerate) {
  using namespace ftxui;

  uint64_t frame = 1;

  Terminal::SetColorSupport(Terminal::Color::TrueColor);

  Component player = Renderer([&] {
    bmp img;
    std::string num =
        std::string(3 - std::min(3, (int)std::to_string(frame).length()), '0') +
        std::to_string(frame);
    std::string filename =
        std::string("images/") + video_id + num + std::string(".bmp");
    img.read(filename.c_str());
    auto c = Canvas(img.info_header.width, img.info_header.height);
    for (unsigned int i = 0; i < img.info_header.width; i++) {
      for (unsigned int j = 0; j < img.info_header.height; j++) {
        c.DrawBlock(i, img.info_header.height - 1 - j, true,
                    img.getPixel(i, j));
      }
    }
    return canvas(std::move(c));
  });

  auto screen = ScreenInteractive::FitComponent();

  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1.0s / framerate);
      // The |shift| variable belong to the main thread. `screen.Post(task)`
      // will execute the update on the thread where |screen| lives (e.g. the
      // main thread). Using `screen.Post(task)` is threadsafe.
      screen.Post([&] { frame++; });
      // After updating the state, request a new frame to be drawn. This is done
      // by simulating a new "custom" event to be handled.
      screen.Post(Event::Custom);

      if (frame >= frame_count)
        refresh_ui_continue = false;
    }
  });

  screen.Loop(player);
  refresh_ui_continue = false;
  refresh_ui.join();

  system((std::string("rm images/") + video_id + std::string("*.bmp")).c_str());

  return 0;
}

void handle_client_action(char buffer[4096]) {
  std::cout << "Client action: " << buffer << std::endl;
}