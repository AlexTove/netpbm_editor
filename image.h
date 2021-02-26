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

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdbool.h>

typedef void *image_handler;

/*!
* @function load_image
* @discussion Loads an image from a file.
* @param image_filename Name / location of the image.
* @return A pointer to an image handler.
*/
image_handler *load_image(char *image_filename);

/*!
* @function unload_image
* @discussion Unloads an image and frees the memory.
* @param image An image handler.
*/
void unload_image(image_handler *image);

/*!
* @function select_image_area
* @discussion Selects an area within an image.
* @param image An image handler.
* @param x1 First coordinate on Ox axis.
* @param y1 First coordinate on Oy axis.
* @param x2 Second coordinate on Ox axis.
* @param y2 Second coordinate on Oy axis.
* @return 0 if an area was selected successfully.
*/
int select_image_area(image_handler image, int x1, int y1, int x2, int y2);

/*!
* @function select_image_all
* @discussion Selects all the image.
* @param image An image handler.
*/
void select_image_all(image_handler image);

/*!
* @function rotate_image
* @discussion Rotate the selected area of an image.
* @param image An image handler.
* @param degrees Number of degrees to rotate de image (multiple of 90).
* @return 0 if function executed successfully.
*/
int rotate_image(image_handler image, int degrees);

/*!
* @function crop_image
* @discussion Crop the selected area of an image.
* @param image An image handler.
*/
void crop_image(image_handler image);

/*!
* @function grayscale_image
* @discussion Turn the selected area of a color image into grayscale.
* @param image An image handler.
* @return 0 if function executed successfully.
*/
int grayscale_image(image_handler image);

/*!
* @function sepia_image
* @discussion Turn the selected area of a color image into sepia.
* @param image An image handler.
* @return 0 if function executed successfully.
*/
int sepia_image(image_handler image);

/*!
* @function save_image
* @discussion Saves an image.
* @param image An image handler.
* @param image_filename A location to save the image.
* @param binary If 1, save the file in binary format,
*				else save in ASCII format.
*/
void save_image(image_handler image, char *image_filename, bool binary);

#endif
