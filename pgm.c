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

#include "pgm.h"
#include "utils.h"

#include <stdlib.h>

pgm_data *load_pgm_image(FILE *image_file, bool binary)
{
	/* Read the PGM image header from given file */

	pgm_data *image = malloc(sizeof(pgm_data));
	image->image_type = IMAGE_TYPE_PGM;
	skip_comments(image_file);

	fscanf(image_file, "%d", &image->width);
	skip_comments(image_file);

	fscanf(image_file, "%d", &image->height);
	skip_comments(image_file);

	fscanf(image_file, "%hd", &image->max_value);
	skip_comments(image_file);

	image->pixel = (unsigned char **)malloc_matrix(image->height, image->width,
												   sizeof(unsigned char));

	/* Read the pixel information from the given file */

	for (int i = 0; i < image->height; ++i) {
		for (int j = 0; j < image->width; ++j) {
			if (binary) {
				image->pixel[i][j] = fgetc(image_file);
			} else {
				int pixel;
				fscanf(image_file, "%d", &pixel);
				image->pixel[i][j] = pixel;
			}
		}
	}

	/* Select the entire area of the picture */

	image->workspace.x1 = 0;
	image->workspace.y1 = 0;
	image->workspace.x2 = image->width;
	image->workspace.y2 = image->height;

	return image;
}

void unload_pgm_image(pgm_data **image)
{
	free_matrix((void **)((*image)->pixel), (*image)->height);
	free(*image);
	*image = NULL;
}

int select_pgm_area(pgm_data *image, int x1, int y1, int x2, int y2)
{
	/* Check if selected area is valid */
	if (x1 < 0 || x2 > image->width || y1 < 0 || y2 > image->height ||
		x1 == x2 || y1 == y2)
		return ERROR_SELECT;

	/* Set the image workspace */
	image->workspace.x1 = x1;
	image->workspace.y1 = y1;
	image->workspace.x2 = x2;
	image->workspace.y2 = y2;

	return 0;
}

void select_pgm_all(pgm_data *image)
{
	image->workspace.x1 = 0;
	image->workspace.y1 = 0;
	image->workspace.x2 = image->width;
	image->workspace.y2 = image->height;
}

void rotate_pgm_90(pgm_data *image)
{
	int width, height;
	width = image->workspace.x2 - image->workspace.x1;
	height = image->workspace.y2 - image->workspace.y1;
	unsigned char **rotated_image;
	rotated_image = (unsigned char **)malloc_matrix(width, height,
													sizeof(unsigned char));

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

void rotate_pgm_180(pgm_data *image)
{
	rotate_pgm_90(image);
	rotate_pgm_90(image);
}

void rotate_pgm_270(pgm_data *image)
{
	rotate_pgm_90(image);
	rotate_pgm_90(image);
	rotate_pgm_90(image);
}

int rotate_pgm(pgm_data *image, int degree)
{
	if (image->workspace.x2 - image->workspace.x1 != image->workspace.y2 -
		image->workspace.y1 && (image->workspace.x1 != 0 ||
		image->workspace.y1 != 0 || image->workspace.x2 != image->width ||
		image->workspace.y2 != image->height))
		return ERROR_ROTATE;

	switch (degree) {
	case 90:
		rotate_pgm_90(image);
		break;

	case 180:
		rotate_pgm_180(image);
		break;

	case 270:
		rotate_pgm_270(image);
		break;

	default:
		return ERROR_ROTATE;
	}
	return 0;
}

void crop_pgm(pgm_data *image)
{
	int new_width, new_height;
	new_width = image->workspace.x2 - image->workspace.x1;
	new_height = image->workspace.y2 - image->workspace.y1;

	unsigned char **cropped_image;
	cropped_image = (unsigned char **)malloc_matrix(new_height, new_width,
													sizeof(unsigned char));

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

void save_pgm_image(FILE *image_file, pgm_data *image, bool binary)
{
	if (binary)
		fprintf(image_file, "P%d\n", FILE_TYPE_PGM_BINARY);
	else
		fprintf(image_file, "P%d\n", FILE_TYPE_PGM_ASCII);

	fprintf(image_file, "%d %d\n", image->width, image->height);
	fprintf(image_file, "%d\n", image->max_value);

	for (int i = 0; i < image->height; ++i) {
		for (int j = 0; j < image->width; ++j) {
			if (binary)
				fputc(image->pixel[i][j], image_file);
			else
				fprintf(image_file, "%d ", (int)image->pixel[i][j]);
		}
		if (!binary)
			fputc('\n', image_file);
	}
}
