#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <definations.h>
#include <stdlib.h>
#include <utils.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
