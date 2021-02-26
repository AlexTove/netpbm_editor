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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#define FILE_TYPE_PBM_ASCII		1
#define FILE_TYPE_PGM_ASCII		2
#define FILE_TYPE_PPM_ASCII		3
#define FILE_TYPE_PBM_BINARY	4
#define FILE_TYPE_PGM_BINARY	5
#define FILE_TYPE_PPM_BINARY	6

#define IMAGE_TYPE_PBM			1
#define IMAGE_TYPE_PGM			2
#define IMAGE_TYPE_PPM			3

#define ERROR_LOAD				1
#define ERROR_SELECT			2
#define ERROR_GRAYSCALE			3
#define ERROR_SEPIA				4
#define ERROR_ROTATE			5

void skip_comments(FILE *image_file);

void **malloc_matrix(int row, int col, int size);
void free_matrix(void **matrix, int row);

#endif
