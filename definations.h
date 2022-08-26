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
#include<stdint.h>
#ifndef __ASCIIART_H__
#define __ASCIIART_H__
/* Make sure we can call this stuff from C++ */
#ifdef __cplusplus
extern "C" {
#endif 

#if defined (_MSC_VER) || defined (__MINGW32__) ||  defined (__GNUC__) && defined (__declspec)
#define ART_APIEXPORT	__declspec(dllexport)
#else
#define	ART_APIEXPORT
#endif
#ifndef INDEX_MATRIX_SZ
#define INDEX_MATRIX_SZ 640 * 480
#endif
/*
 * Current rendering state and the glyph table is recorded on a instance
 * of the following structure.
 */
typedef struct count_ {
	int count_i, count_k;
	int tree_i;
} count_t;

void getstbi(const char *fname);
#endif /* ART_ENABLE_STB_IMAGE */

#ifdef __cplusplus
}
#endif 
