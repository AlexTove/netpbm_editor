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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE	1024

void load(image_handler *image, char params[])
{
	char image_filename[BUFFER_SIZE];
	int matches = sscanf(params, "%s", image_filename);

	if (matches < 1) {
		printf("Invalid command\n");
		return;
	}

	if (*image)
		unload_image(image);

	*image = load_image(image_filename);
	if (!(*image))
		printf("Failed to load %s\n", image_filename);
	else
		printf("Loaded %s\n", image_filename);
}

void save(image_handler *image, char params[])
{
	char image_filename[BUFFER_SIZE], image_type[BUFFER_SIZE];
	int matches = sscanf(params, "%s %s", image_filename, image_type);

	if (matches < 1) {
		printf("Invalid command\n");
		return;
	}

	if (!(*image)) {
		printf("No image loaded\n");
		return;
	}

	bool binary = 1;
	if (matches == 2 && !strcmp("ascii", image_type))
		binary = 0;

	save_image(*image, image_filename, binary);
	printf("Saved %s\n", image_filename);
}

void select_area(image_handler image, char params[])
{
	int x1, y1, x2, y2;
	char all_selection[BUFFER_SIZE];
	int coord_match = sscanf(params, "%d %d %d %d", &x1, &y1, &x2, &y2);
	int select_all_match = sscanf(params, "%s", all_selection);

	if (coord_match != 4 && select_all_match != 1) {
		printf("Invalid command\n");
	} else if (!image) {
		printf("No image loaded\n");
	} else if (coord_match == 4) {
		if (x1 > x2) {
			int aux = x1;
			x1 = x2;
			x2 = aux;
		}

		if (y1 > y2) {
			int aux = y1;
			y1 = y2;
			y2 = aux;
		}
		int error = select_image_area(image, x1, y1, x2, y2);
		if (error)
			printf("Invalid set of coordinates\n");
		else
			printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	} else if (select_all_match == 1 && !strcmp("ALL", all_selection)) {
		select_image_all(image);
		printf("Selected ALL\n");
	} else {
		printf("Invalid command\n");
	}
}

void rotate(image_handler image, char params[])
{
	int degree;
	int matches = sscanf(params, "%d", &degree);

	if (matches != 1) {
		printf("Invalid command\n");
		return;
	}

	if (!image) {
		printf("No image loaded\n");
		return;
	}

	if (degree % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	int error = rotate_image(image, degree % 360);

	if (!error)
		printf("Rotated %d\n", degree);
	else
		printf("Invalid set of coordinates\n");
}

void crop(image_handler image)
{
	if (!image) {
		printf("No image loaded\n");
		return;
	}

	crop_image(image);
	printf("Image cropped\n");
}

void grayscale(image_handler image)
{
	if (!image) {
		printf("No image loaded\n");
		return;
	}

	int error = grayscale_image(image);

	if (error != 0) {
		printf("Grayscale filter not available\n");
		return;
	}
	printf("Grayscale filter applied\n");
}

void sepia(image_handler image)
{
	if (!image) {
		printf("No image loaded\n");
		return;
	}

	int error = sepia_image(image);

	if (error != 0) {
		printf("Sepia filter not available\n");
		return;
	}
	printf("Sepia filter applied\n");
}

int main(void)
{
	image_handler image = NULL;
	char cmd[BUFFER_SIZE];

	while (scanf("%s", cmd) != EOF) {
		char params[BUFFER_SIZE];
		fgets(params, BUFFER_SIZE, stdin);

		/* Parse the command and call the appropriate function*/
		if (!strcmp("EXIT", cmd)) {
			if (!image)
				printf("No image loaded\n");
			else
				unload_image(&image);
			break;
		} else if (!strcmp("LOAD", cmd)) {
			load(&image, params);
		} else if (!strcmp("SELECT", cmd)) {
			select_area(image, params);
		} else if (!strcmp("ROTATE", cmd)) {
			rotate(image, params);
		} else if (!strcmp("CROP", cmd)) {
			crop(image);
		} else if (!strcmp("GRAYSCALE", cmd)) {
			grayscale(image);
		} else if (!strcmp("SEPIA", cmd)) {
			sepia(image);
		} else if (!strcmp("SAVE", cmd)) {
			save(&image, params);
		} else {
			printf("Invalid command\n");
		}
	}
	return 0;
}
