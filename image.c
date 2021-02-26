/* MIT License
*
* Copyright (c) 2021 Toader Alexandru(315CA)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
**/

#include "image.h"
#include "utils.h"
#include "pgm.h"
#include "ppm.h"

#include <stdio.h>
#include <string.h>

image_handler *load_image(char image_filename[])
{
	/* Open the file and get the magic word*/
	FILE *image_file = fopen(image_filename, "rb");
	if (!image_file)
		return NULL;

	skip_comments(image_file);
	char magicword[3];
	fgets(magicword, sizeof(magicword), image_file);
	unsigned char file_image_type = magicword[1] - '0';

	image_handler image;

	switch (file_image_type) {
	case FILE_TYPE_PGM_ASCII:
		image = load_pgm_image(image_file, 0);
		break;
	case FILE_TYPE_PGM_BINARY:
		image = load_pgm_image(image_file, 1);
		break;
	case FILE_TYPE_PPM_ASCII:
		image = load_ppm_image(image_file, 0);
		break;
	case FILE_TYPE_PPM_BINARY:
		image = load_ppm_image(image_file, 1);
		break;
	}

	fclose(image_file);
	return image;
}

void unload_image(image_handler *image)
{
	if (!(*image))
		return;

	unsigned char image_type = *((unsigned char *)(*image));

	switch (image_type) {
	case IMAGE_TYPE_PGM:
		unload_pgm_image((pgm_data **)image);
		break;
	case IMAGE_TYPE_PPM:
		unload_ppm_image((ppm_data **)image);
		break;
	}
}

int select_image_area(image_handler image, int x1, int y1, int x2, int y2)
{
	if (!image)
		return ERROR_SELECT;

	unsigned char image_type = *((unsigned char *)(image));

	switch (image_type) {
	case IMAGE_TYPE_PGM:
		return select_pgm_area((pgm_data *)image, x1, y1, x2, y2);
	case IMAGE_TYPE_PPM:
		return select_ppm_area((ppm_data *)image, x1, y1, x2, y2);
	}
}

void select_image_all(image_handler image)
{
	if (!image)
		return;

	unsigned char image_type = *((unsigned char *)(image));

	switch (image_type) {
	case IMAGE_TYPE_PGM:
		select_pgm_all((pgm_data *)image);
		break;
	case IMAGE_TYPE_PPM:
		select_ppm_all((ppm_data *)image);
		break;
	}
}

int rotate_image(image_handler image, int degree)
{
	if (!image)
		return ERROR_ROTATE;

	unsigned char image_type = *((unsigned char *)(image));

	if (degree < 0)
		degree = 360 + degree;
	else if (degree == 0)
		return 0;

	switch (image_type) {
	case IMAGE_TYPE_PGM:
		return rotate_pgm((pgm_data *)image, degree);
	case IMAGE_TYPE_PPM:
		return rotate_ppm((ppm_data *)image, degree);
	}
}

void crop_image(image_handler image)
{
	if (!image)
		return;

	unsigned char image_type = *((unsigned char *)(image));

	switch (image_type) {
	case IMAGE_TYPE_PGM:
		crop_pgm((pgm_data *)image);
		break;
	case IMAGE_TYPE_PPM:
		crop_ppm((ppm_data *)image);
		break;
	}
}

int grayscale_image(image_handler image)
{
	if (!image)
		return ERROR_GRAYSCALE;

	unsigned char image_type = *((unsigned char *)(image));

	if (image_type != IMAGE_TYPE_PPM)
		return ERROR_GRAYSCALE;

	grayscale_ppm((ppm_data *)image);
	return 0;
}

int sepia_image(image_handler image)
{
	if (!image)
		return ERROR_SEPIA;

	unsigned char image_type = *((unsigned char *)(image));

	if (image_type != IMAGE_TYPE_PPM)
		return ERROR_SEPIA;

	sepia_ppm((ppm_data *)image);
	return 0;
}

void save_image(image_handler image, char image_filename[], bool binary)
{
	if (!image)
		return;

	FILE *image_file = fopen(image_filename, "wb");
	if (!image_file)
		return;

	unsigned char image_type = *((unsigned char *)image);

	switch (image_type) {
	case IMAGE_TYPE_PGM:
		save_pgm_image(image_file, (pgm_data *)image, binary);
		break;
	case IMAGE_TYPE_PPM:
		save_ppm_image(image_file, (ppm_data *)image, binary);
		break;
	}
	fclose(image_file);
}
