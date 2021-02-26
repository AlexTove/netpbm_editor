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

#ifndef __PGM_H__
#define __PGM_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct pgm_selected_area {
	int x1, y1;
	int x2, y2;
} pgm_selected_area;

typedef struct pgm_data {
	unsigned char image_type;
	int width, height;
	short max_value;
	pgm_selected_area workspace;
	unsigned char **pixel;
} pgm_data;

pgm_data *load_pgm_image(FILE *image_file, bool binary);
void unload_pgm_image(pgm_data **image);
int select_pgm_area(pgm_data *image, int x1, int y1, int x2, int y2);
void select_pgm_all(pgm_data *image);
void rotate_pgm_90(pgm_data *image);
void rotate_pgm_180(pgm_data *image);
void rotate_pgm_270(pgm_data *image);
int rotate_pgm(pgm_data *image, int degree);
void crop_pgm(pgm_data *image);
void save_pgm_image(FILE *image_file, pgm_data *image, bool binary);

#endif
