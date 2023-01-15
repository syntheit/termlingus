//
// Created by bertoc2 on 1/14/2023.
//

#include "bmp.h"

std::vector<uint8_t> bmp::deserialize(std::vector<bmp_packet> packets, uint16_t width, uint16_t height) {
  std::vector<uint8_t> newData = std::vector<uint8_t>(width * height * 3);
  for (uint32_t i = 0; i < width * height * 3; i++) {
    data[i] = packets[i / 4096].data[i % 4096];
  }

  return newData;
}

std::vector<bmp_packet> bmp::serialize() {
  std::vector<bmp_packet> ret(data.size() / 4096 + 1, {0});
  for (uint32_t i = 0; i < data.size(); i++) {
    ret[i / 4096].data[i % 4096] = data[i];
  }

  return ret;
}


ftxui::Color bmp::getPixel(uint32_t x, uint32_t y) {
  uint32_t channels = info_header.bitCount / 8;

  uint8_t b = data[channels * (y * info_header.width + x) + 0];
  uint8_t g = data[channels * (y * info_header.width + x) + 1];
  uint8_t r = data[channels * (y * info_header.width + x) + 2];

  return ftxui::Color::RGB(r, g, b);
}

void bmp::read(const char *fname) {
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

uint32_t bmp::make_stride_aligned(uint32_t align_stride) {
  uint32_t new_stride = row_stride;
  while (new_stride % align_stride != 0) {
    new_stride++;
  }
  return new_stride;
}

void bmp::check_color_header(bmp_color_header &old_color_header) {
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
