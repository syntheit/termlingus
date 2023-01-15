#include <ftxui/dom/elements.hpp>  // for canvas, Element, separator, hbox, operator|, border
#include <string>   // for string, basic_string
#include <fstream>

#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

#include "bmp.h"
#include "command.h"
#include <cassert>

using namespace ftxui;

Component Thumbnail(std::string& filepath) {

  //system((std::string("ffmpeg -pix_fmt bgr24 -i \"") + filepath + std::string("\" tb.bmp")).c_str());

  assert(false);

  return Renderer([&] {
    bmp img;
    img.read("tb.bmp");
    auto c = Canvas(img.info_header.width, img.info_header.height);
    for (unsigned int i = 0; i < img.info_header.width; i++) {
      for (unsigned int j = 0; j < img.info_header.height; j++) {
        c.DrawBlock(i, img.info_header.height - j - 1, true, img.getPixel(i, j));
      }
    }
    return canvas(std::move(c));
  });
}