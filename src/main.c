#include <stdio.h>
#include "image.h"

extern int width, height;
extern int invert;
extern int colorfill, usecolors, colorDepth;

int main(int argc, char** argv) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

	width = 211;
	height = 546;
	colorDepth = 24;
	usecolors = 1;
	colorfill = 1;
	invert = 1;

	error_collector errors;
	errors.jpeg_status = 0;
	errors.png_status = 0;

	decompress_jpeg(input, output, &errors);
}
