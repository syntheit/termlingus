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

struct bmp {
public:
  bmp_header file_header;
  bmp_info_header info_header;
  bmp_color_header color_header;
  std::vector<uint8_t> data;

  ftxui::Color getPixel(uint32_t x, uint32_t y) {
    uint32_t channels = info_header.bitCount / 8;

    uint8_t b = data[channels * (y * info_header.width + x) + 0];
    uint8_t g = data[channels * (y * info_header.width + x) + 1];
    uint8_t r = data[channels * (y * info_header.width + x) + 2];

    return ftxui::Color::RGB(r, g, b);
  }

  void read(const char *fname) {
    std::ifstream inp{ fname, std::ios_base::binary };
    if (inp) {
      inp.read((char*)&file_header, sizeof(file_header));
      if(file_header.bmpSig != 0x4D42) {
        throw std::runtime_error("Error! Unrecognized file format.");
      }
      inp.read((char*)&info_header, sizeof(bmp_info_header));

      // The BMPColorHeader is used only for transparent images
      if(info_header.bitCount == 32) {
        // Check if the file has bit mask color information
        if(info_header.size >= (sizeof(bmp_info_header) + sizeof(bmp_color_header))) {
          inp.read((char*)&color_header, sizeof(bmp_color_header));
          // Check if the pixel data is stored as BGRA and if the color space type is sRGB
          check_color_header(color_header);
        } else {
          std::cerr << "Warning! The file \"" << fname << "\" does not seem to contain bit mask information\n";
          throw std::runtime_error("Error! Unrecognized file format.");
        }
      }

      // Jump to the pixel data location
      inp.seekg(file_header.dataOff, inp.beg);

      // Adjust the header fields for output.
      // Some editors will put extra info in the image file, we only save the headers and the data.
      if(info_header.bitCount == 32) {
        info_header.size = sizeof(bmp_info_header) + sizeof(bmp_color_header);
        file_header.dataOff = sizeof(bmp_header) + sizeof(bmp_info_header) + sizeof(bmp_color_header);
      } else {
        info_header.size = sizeof(bmp_info_header);
        file_header.dataOff = sizeof(bmp_header) + sizeof(bmp_info_header);
      }
      file_header.fSize = file_header.dataOff;

      if (info_header.height < 0) {
        throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
      }

      data.resize(info_header.width * info_header.height * info_header.bitCount / 8);

      // Here we check if we need to take into account row padding
      if (info_header.width % 4 == 0) {
        inp.read((char*)data.data(), data.size());
        file_header.fSize += data.size();
      }
      else {
        row_stride = info_header.width * info_header.bitCount / 8;
        uint32_t new_stride = make_stride_aligned(4);
        std::vector<uint8_t> padding_row(new_stride - row_stride);

        for (int y = 0; y < info_header.height; ++y) {
          inp.read((char*)(data.data() + row_stride * y), row_stride);
          inp.read((char*)padding_row.data(), padding_row.size());
        }
        file_header.fSize += data.size() + info_header.height * padding_row.size();
      }
    }
    else {
      throw std::runtime_error("Unable to open the input image file.");
    }
  }
private:
  uint32_t row_stride;

  // Add 1 to the row_stride until it is divisible with align_stride
  uint32_t make_stride_aligned(uint32_t align_stride) {
    uint32_t new_stride = row_stride;
    while (new_stride % align_stride != 0) {
      new_stride++;
    }
    return new_stride;
  }

  void check_color_header(bmp_color_header &old_color_header) {
    bmp_color_header expected_color_header;
    if(expected_color_header.redMask != old_color_header.redMask ||
       expected_color_header.blueMask != old_color_header.blueMask ||
       expected_color_header.greenMask != old_color_header.greenMask ||
       expected_color_header.alphaMask != old_color_header.alphaMask) {
      throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
    }
    if(expected_color_header.colorSpace != old_color_header.colorSpace) {
      throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
    }
  }
}__attribute__((packed));


#endif //TERMLINGUS_BMP_H
