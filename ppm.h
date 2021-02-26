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

#ifndef __PPM_H__
#define __PPM_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct ppm_pixel_data {
	unsigned char red, green, blue;
} ppm_pixel_data;

typedef struct ppm_selected_area {
	int x1, y1;
	int x2, y2;
} ppm_selected_area;

typedef struct ppm_data {
	unsigned char image_type;
	int width, height;
	short max_value;
	ppm_selected_area workspace;
	ppm_pixel_data **pixel;
} ppm_data;

ppm_data *load_ppm_image(FILE *image_file, bool binary);
void unload_ppm_image(ppm_data **image);
int select_ppm_area(ppm_data *image, int x1, int y1, int x2, int y2);
void select_ppm_all(ppm_data *image);
void rotate_ppm_90(ppm_data *image);
void rotate_ppm_180(ppm_data *image);
void rotate_ppm_270(ppm_data *image);
int rotate_ppm(ppm_data *image, int degree);
void crop_ppm(ppm_data *image);
void grayscale_ppm(ppm_data *image);
void sepia_ppm(ppm_data *image);
void save_ppm_image(FILE *image_file, ppm_data *image, bool binary);

#endif
