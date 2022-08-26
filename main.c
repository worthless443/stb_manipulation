/*
* ASCII Art: Real-time ASCII Art Rendering Library.
* Copyright (C) PixLab. https://pixlab.io/art
* Version 1.2
* For information on licensing, redistribution of this file, and for a DISCLAIMER OF ALL WARRANTIES
* please contact:
*       support@pixlab.io
*       contact@pixlab.io
* or visit:
*     https://pixlab.io/art
*/
/*
* An Implementation based on the paper:
* > N. Markus, M. Fratarcangeli, I. S. Pandzic and J. Ahlberg, "Fast Rendering of Image Mosaics and ASCII Art", Computer Graphics Forum, 2015, http://dx.doi.org/10.1111/cgf.12597
*/
#include <string.h>
#include <stdint.h>
#include <stdio.h>
//#include <ascii_art.h>
#include <stdlib.h>
#include <utils.h>
/*
 * This is the output hex model generated during the training phase. 
 * It contains both the codebook and the decision tree that let you
 * render your images or video frames at Real-time.
 *
 * The model can be downloaded from: https://pixlab.io/art
 */
static const unsigned char zBin[] = {
#if __has_include("ascii_art.hex") 
#include "ascii_art.hex"
#define  ascii_include 1
#else
#define ascii_include 0
#endif
};
///*
// * Glyph table.
// */
// const unsigned char glyph_char_table[] =
//{
//	' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
//	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
//	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
//	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
//	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
//	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
//};
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
/*
* CAPIREF: Refer to the official documentation for the main purpose of this interface.
*/

char * AsciiArtLoadImage(const char * zPath, int * pWidth, int * pHeight)
{
	unsigned char *zBlob;
	int c;
	zBlob = stbi_load(zPath,pWidth, pHeight, &c, 1);
	return zBlob;
}

void getstbi(const char *fname) {
	int arr[101];
	const char *matrix = stbi_parse(fname);
	int x;
	if(matrix!=NULL) x = *matrix;
	else return;
	for(int i=0,k=0;i<50;i++,k++) { 
	      arr[i] = matrix[i];
	}
	arr[101] = -1;
	if(check_zeros(arr)>10) printf("prolly got an png file or error in parsing\n");
}

int main(int argc, char **argv) {
	getstbi(argv[1]);
}
 //#endif /* STB_IMAGE_IMPLEMENTATION */
