#include<stdint.h>
#ifndef __ASCIIART_H__
#define __ASCIIART_H__
/* Make sure we can call this stuff from C++ */
#ifdef __cplusplus
extern "C" {
#endif 

// define this for std_image to load symbols for use in main.c

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
const char *stbi_parse(const char *fname);

#endif /* ART_ENABLE_STB_IMAGE */

#ifdef __cplusplus
}
#endif 

