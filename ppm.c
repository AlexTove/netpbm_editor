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

#include <stdlib.h>
#include <math.h>

#include "ppm.h"
#include "utils.h"

ppm_data *load_ppm_image(FILE *image_file, bool binary)
{
	ppm_data *image = malloc(sizeof(ppm_data));
	image->image_type = IMAGE_TYPE_PPM;
	skip_comments(image_file);

	fscanf(image_file, "%d", &image->width);
	skip_comments(image_file);

	fscanf(image_file, "%d", &image->height);
	skip_comments(image_file);

	fscanf(image_file, "%hd", &image->max_value);
	skip_comments(image_file);

	image->pixel = (ppm_pixel_data **)malloc_matrix(image->height, image->width,
								 sizeof(ppm_pixel_data));

	for (int i = 0; i < image->height; ++i) {
		for (int j = 0; j < image->width; ++j) {
			if (binary) {
				image->pixel[i][j].red = fgetc(image_file);
				image->pixel[i][j].green = fgetc(image_file);
				image->pixel[i][j].blue = fgetc(image_file);
			} else {
				int red, green, blue;
				fscanf(image_file, "%d%d%d", &red, &green, &blue);
				image->pixel[i][j].red = red;
				image->pixel[i][j].green = green;
				image->pixel[i][j].blue = blue;
			}
		}
	}

	image->workspace.x1 = 0;
	image->workspace.y1 = 0;
	image->workspace.x2 = image->width;
	image->workspace.y2 = image->height;

	return image;
}

void unload_ppm_image(ppm_data **image)
{
	free_matrix((void **)((*image)->pixel), (*image)->height);
	free(*image);
	*image = NULL;
}

int select_ppm_area(ppm_data *image, int x1, int y1, int x2, int y2)
{
	if (x1 < 0 || x2 > image->width || y1 < 0 || y2 > image->height ||
		x1 == x2 || y1 == y2)
		return ERROR_SELECT;

	image->workspace.x1 = x1;
	image->workspace.y1 = y1;
	image->workspace.x2 = x2;
	image->workspace.y2 = y2;

	return 0;
}

void select_ppm_all(ppm_data *image)
{
	image->workspace.x1 = 0;
	image->workspace.y1 = 0;
	image->workspace.x2 = image->width;
	image->workspace.y2 = image->height;
}

void rotate_ppm_90(ppm_data *image)
{
	int width, height;
	width = image->workspace.x2 - image->workspace.x1;
	height = image->workspace.y2 - image->workspace.y1;
	ppm_pixel_data **rotated_image;
	rotated_image = (ppm_pixel_data **)malloc_matrix(width, height,
													sizeof(ppm_pixel_data));

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rotated_image[j][height - i - 1] =
			image->pixel[image->workspace.y1 + i][image->workspace.x1 + j];

	if (image->workspace.x2 - image->workspace.x1 == image->workspace.y2 -
		image->workspace.y1) {
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				image->pixel[image->workspace.y1 + i][image->workspace.x1 + j] =
				rotated_image[i][j];

		free_matrix((void **)(rotated_image), height);
	} else {
		free_matrix((void **)(image->pixel), image->height);
		image->pixel = rotated_image;
		image->height = width;
		image->width = height;
		image->workspace.x2 = height;
		image->workspace.y2 = width;
	}
}

void rotate_ppm_180(ppm_data *image)
{
	rotate_ppm_90(image);
	rotate_ppm_90(image);
}

void rotate_ppm_270(ppm_data *image)
{
	rotate_ppm_90(image);
	rotate_ppm_90(image);
	rotate_ppm_90(image);
}

int rotate_ppm(ppm_data *image, int degree)
{
	if (image->workspace.x2 - image->workspace.x1 != image->workspace.y2 -
		image->workspace.y1 && (image->workspace.x1 != 0 ||
		image->workspace.y1 != 0 || image->workspace.x2 != image->width ||
		image->workspace.y2 != image->height))
		return ERROR_ROTATE;

	switch (degree) {
	case 90:
		rotate_ppm_90(image);
		break;

	case 180:
		rotate_ppm_180(image);
		break;

	case 270:
		rotate_ppm_270(image);
		break;

	default:
		return ERROR_ROTATE;
	}
	return 0;
}

void crop_ppm(ppm_data *image)
{
	int new_width, new_height;
	new_width = image->workspace.x2 - image->workspace.x1;
	new_height = image->workspace.y2 - image->workspace.y1;

	ppm_pixel_data **cropped_image;
	cropped_image = (ppm_pixel_data **)malloc_matrix(new_height, new_width,
													 sizeof(ppm_pixel_data));

	for (int i = 0; i < new_height; ++i)
		for (int j = 0; j < new_width; ++j)
			cropped_image[i][j] =
				image->pixel[image->workspace.y1 + i][image->workspace.x1 + j];

	free_matrix((void **)(image->pixel), image->height);

	image->pixel = cropped_image;
	image->width = new_width;
	image->height = new_height;
	image->workspace.x1 = 0;
	image->workspace.y1 = 0;
	image->workspace.x2 = image->width;
	image->workspace.y2 = image->height;
}

void grayscale_ppm(ppm_data *image)
{
	for (int i = image->workspace.y1; i < image->workspace.y2; ++i) {
		for (int j = image->workspace.x1; j < image->workspace.x2; ++j) {
			double new_pixel_value;
			new_pixel_value = round(((double)image->pixel[i][j].red +
									(double)image->pixel[i][j].green +
									(double)image->pixel[i][j].blue) / 3.0);
			image->pixel[i][j].red = (unsigned char)new_pixel_value;
			image->pixel[i][j].green = (unsigned char)new_pixel_value;
			image->pixel[i][j].blue = (unsigned char)new_pixel_value;
		}
	}
}

void sepia_ppm(ppm_data *image)
{
	for (int i = image->workspace.y1; i < image->workspace.y2; ++i) {
		for (int j = image->workspace.x1; j < image->workspace.x2; ++j) {
			double red, green, blue;
			double new_red, new_green, new_blue;
			red = image->pixel[i][j].red;
			green = image->pixel[i][j].green;
			blue = image->pixel[i][j].blue;
			new_red = round(0.393 * red + 0.769 * green + 0.189 * blue);
			new_green = round(0.349 * red + 0.686 * green +  0.168 * blue);
			new_blue = round(0.272 * red + 0.534 * green + 0.131 * blue);
			image->pixel[i][j].red = (255 < new_red) ? 255 : new_red;
			image->pixel[i][j].green = (255 < new_green) ? 255 : new_green;
			image->pixel[i][j].blue = (255 < new_blue) ? 255 : new_blue;
		}
	}
}

void save_ppm_image(FILE *image_file, ppm_data *image, bool binary)
{
	if (binary)
		fprintf(image_file, "P%d\n", FILE_TYPE_PPM_BINARY);
	else
		fprintf(image_file, "P%d\n", FILE_TYPE_PPM_ASCII);

	fprintf(image_file, "%d %d\n", image->width, image->height);
	fprintf(image_file, "%d\n", image->max_value);

	for (int i = 0; i < image->height; ++i) {
		for (int j = 0; j < image->width; ++j) {
			if (binary) {
				fputc(image->pixel[i][j].red, image_file);
				fputc(image->pixel[i][j].green, image_file);
				fputc(image->pixel[i][j].blue, image_file);
			} else {
				fprintf(image_file, "%d %d %d ", (int)image->pixel[i][j].red,
						(int)image->pixel[i][j].green,
						(int)image->pixel[i][j].blue);
			}
		}
		if (!binary)
			fputc('\n', image_file);
	}
}
