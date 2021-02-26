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

#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void skip_comments(FILE *image_file)
{
	char byte;

	while (byte = fgetc(image_file)) {
		if (byte == '#') {
			while (1) {
				char comm = fgetc(image_file);
				if (comm == '\n')
					break;
			}
		} else if (!isspace(byte) && byte != '\n') {
			fseek(image_file, -1, SEEK_CUR);
			break;
		}
	}
}

void **malloc_matrix(int row, int col, int size)
{
	void **matrix = malloc(row * sizeof(void *));
	for (int i = 0; i < row; ++i)
		matrix[i] = malloc(col * size);
	return matrix;
}

void free_matrix(void **matrix, int row)
{
	for (int i = 0; i < row; ++i)
		free(matrix[i]);
	free(matrix);
}
