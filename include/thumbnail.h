//
// Created by bertoc2 on 1/14/2023.
//

#ifndef TERMLINGUS_THUMBNAIL_H
#define TERMLINGUS_THUMBNAIL_H

#include <ftxui/dom/elements.hpp>  // for canvas, Element, separator, hbox, operator|, border
#include <string>   // for string, basic_string
#include <fstream>

#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

ftxui::Component Thumbnail(const std::string& filepath);

#endif //TERMLINGUS_THUMBNAIL_H
