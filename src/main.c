#include <stdio.h>
#include "image.h"

// Workaround for compiling C into C++
void decompress_jpeg(FILE *fin, FILE *fout, error_collector *errors) asm ("decompress_jpeg");

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
