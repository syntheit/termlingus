//
// Created by bertoc2 on 1/14/2023.
//

#ifndef TERMLINGUS_BMP_H
#define TERMLINGUS_BMP_H

#include <stdint.h>
#include <fstream>

#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"                    // for Canvas
#include "ftxui/screen/color.hpp"  // for Color, Color::Red, Color::Blue, Color::Green, ftxui

struct bmp_header {
  uint16_t bmpSig;
  uint32_t fSize;
  uint16_t res0;
  uint16_t res1;
  uint32_t dataOff;
}__attribute__((packed));

struct bmp_info_header {
  uint32_t size;
  int32_t width;
  int32_t height;

  uint16_t planes;
  uint16_t bitCount;
  uint32_t compression;
  uint32_t imageSize;
  int32_t xPPM;
  int32_t yPPM;
  uint32_t colorsUsed;
  uint32_t colorsImportant;
}__attribute__((packed));

struct bmp_color_header {
  uint32_t redMask = 0x00ff0000;
  uint32_t greenMask = 0x0000ff00;
  uint32_t blueMask = 0x000000ff;
  uint32_t alphaMask = 0xff000000;
  uint32_t colorSpace = 0x73524742;
  uint32_t unused[16]{ 0 };
}__attribute__((packed));

typedef struct {char data[4096];}__attribute__((packed)) bmp_packet;

struct bmp_start_packet {
  uint16_t type{0};
  uint16_t sizeX;
  uint16_t sizeY;
  uint8_t data[4090];
}__attribute__((packed));

struct bmp_data_packet {
  uint16_t type{(uint16_t)-1};
  uint8_t data[4094];
}__attribute__((packed));

struct bmp {
public:
  bmp_header file_header;
  bmp_info_header info_header;
  bmp_color_header color_header;
  std::vector<uint8_t> data;

  ftxui::Color getPixel(uint32_t x, uint32_t y);

  std::vector<bmp_packet> serialize();
  std::vector<uint8_t> deserialize(std::vector<bmp_packet> packets, uint16_t width, uint16_t height);

  void read(const char *fname);

private:
  uint32_t row_stride = sizeof(bmp_data_packet);

  uint32_t make_stride_aligned(uint32_t align_stride);

  void check_color_header(bmp_color_header &old_color_header);
}__attribute__((packed));


#endif //TERMLINGUS_BMP_H
