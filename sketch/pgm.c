/* Demo for writing a PGM image file.

Each PGM image consists of the following:

A "magic number" for identifying the file type. A pgm image's magic number is the two characters "P5".
Whitespace (blanks, TABs, CRs, LFs).
A width, formatted as ASCII characters in decimal.
Whitespace.
A height, again in ASCII decimal.
Whitespace.
The maximum gray value (Maxval), again in ASCII decimal. Must be less than 65536, and more than zero.
A single whitespace character (usually a newline).
A raster of Height rows, in order from top to bottom. Each row consists of Width gray values, in order from left to right. Each gray value is a number from 0 through Maxval, with 0 being black and Maxval being white. Each gray value is represented in pure binary by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes. The most significant byte is first.
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define H 480
#define W 640

int main() {

	double image[H][W], max = 0;
	unsigned char imageOut[H][W];

	// Make something interesting
	for (int i = 0; i < H; ++i){
		for (int j = 0; j < W; ++j){
			image[i][j] = fabs(sin( ((double)j) / (H/10)) * ((double)i));
			if (image[i][j] > max) max = image[i][j];
		}
	}

	//Scale 0 -> 255
	for (int i = 0; i < H; ++i){
		for (int j = 0; j < W; ++j){
			imageOut[i][j] = (image[i][j] / max) * 255;
		}
	}

	FILE *ofp = fopen("image.pgm", "wb");
	if (ofp == NULL) {
		fprintf(stderr, "Cannot write image.pgm\n");
		exit(1);
	}

	//Write header
	fprintf(ofp,"P5 %d %d 255\n", W, H);
	//Write bytes
	fwrite(imageOut, 1, H*W, ofp);

	fclose(ofp); // after the program finishes you can open the image file with "xv image.pgm" or any editor
}
