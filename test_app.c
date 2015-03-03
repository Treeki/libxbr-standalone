#include "filters.h"
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>


int main(int argc, char **argv) {
	FILE *fp;
	unsigned char pngHeader[8];
	png_structp png;
	png_infop info;
	png_bytep *rowPointers;
	uint8_t *inBuffer, *outBuffer;
	int width, height;
	int scaleType, scaleFactor;
	int i;
	xbr_data *xbrData;
	xbr_params xbrParams;

	if (argc != 4) {
		printf("Usage: %s [input.png] [output.png] [xbr2x|xbr3x|xbr4x|hq2x|hq3x|hq4x]\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (strcmp(argv[3], "xbr2x") == 0) {
		scaleType = 1; scaleFactor = 2;
	} else if (strcmp(argv[3], "xbr3x") == 0) {
		scaleType = 1; scaleFactor = 3;
	} else if (strcmp(argv[3], "xbr4x") == 0) {
		scaleType = 1; scaleFactor = 4;
	} else if (strcmp(argv[3], "hq2x") == 0) {
		scaleType = 2; scaleFactor = 2;
	} else if (strcmp(argv[3], "hq3x") == 0) {
		scaleType = 2; scaleFactor = 3;
	} else if (strcmp(argv[3], "hq4x") == 0) {
		scaleType = 2; scaleFactor = 4;
	} else {
		printf("Unrecognised scale mode\n");
		return EXIT_FAILURE;
	}


	/* Try to read input file */
	fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("Could not open input file\n");
		return EXIT_FAILURE;
	}

	fread(pngHeader, 1, 8, fp);
	if (png_sig_cmp(pngHeader, 0, 8)) {
		printf("Input file does not look like a PNG\n");
		return EXIT_FAILURE;
	}


	/* Make PNG */
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		printf("Could not create PNG read struct\n");
		return EXIT_FAILURE;
	}

	info = png_create_info_struct(png);
	if (!info) {
		printf("Could not create PNG info struct\n");
		return EXIT_FAILURE;
	}

	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);
	png_read_png(png, info, PNG_TRANSFORM_BGR, NULL);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);

	if (png_get_bit_depth(png, info) != 8 || png_get_channels(png, info) != 4) {
		printf("PNG must be 32-bit\n");
		return EXIT_FAILURE;
	}


	/* Extract data */
	inBuffer = malloc(width * height * 4);

	rowPointers = png_get_rows(png, info);
	for (i = 0; i < height; i++) {
		memcpy(inBuffer + (i * width * 4), rowPointers[i], width * 4);
	}


	png_destroy_read_struct(&png, &info, NULL);
	fclose(fp);


	/* CONVERT IT! */
	outBuffer = malloc(width * scaleFactor * height * scaleFactor * 4);

	xbrData = malloc(sizeof(xbr_data));
	xbr_init_data(xbrData);

	xbrParams.data = xbrData;
	xbrParams.input = inBuffer;
	xbrParams.output = outBuffer;
	xbrParams.inWidth = width;
	xbrParams.inHeight = height;
	xbrParams.inPitch = width * 4;
	xbrParams.outPitch = width * scaleFactor * 4;

	if (scaleType == 1) {
		switch (scaleFactor) {
			case 2: xbr_filter_xbr2x(&xbrParams); break;
			case 3: xbr_filter_xbr3x(&xbrParams); break;
			case 4: xbr_filter_xbr4x(&xbrParams); break;
		}
	} else if (scaleType == 2) {
		switch (scaleFactor) {
			case 2: xbr_filter_hq2x(&xbrParams); break;
			case 3: xbr_filter_hq3x(&xbrParams); break;
			case 4: xbr_filter_hq4x(&xbrParams); break;
		}
	}


	/* Generate a new PNG */
	fp = fopen(argv[2], "wb");
	if (!fp) {
		printf("Could not open output file for writing\n");
		return EXIT_FAILURE;
	}

	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		printf("Could not create PNG write struct\n");
		return EXIT_FAILURE;
	}

	info = png_create_info_struct(png);
	if (!info) {
		printf("Could not create info write struct\n");
		return EXIT_FAILURE;
	}

	if (setjmp(png_jmpbuf(png))) {
		png_destroy_write_struct(&png, &info);
		fclose(fp);
		printf("Error writing PNG\n");
		return EXIT_FAILURE;
	}

	png_init_io(png, fp);
	png_set_IHDR(png, info, width*scaleFactor, height*scaleFactor, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	rowPointers = malloc(sizeof(png_bytep) * height * scaleFactor);
	for (i = 0; i < (height * scaleFactor); i++) {
		rowPointers[i] = outBuffer + (i * width * scaleFactor * 4);
	}

	png_set_rows(png, info, rowPointers);
	png_write_png(png, info, PNG_TRANSFORM_BGR, NULL);
	png_destroy_write_struct(&png, &info);

	fclose(fp);

	return EXIT_SUCCESS;
}


