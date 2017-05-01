#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.cpp"
#include "render.h"
#include "pixelray.h"
#include "pixelcolor.h"

int main(int argc, char *argv[]) {
	int mode = 0;
	parse(argv[2]);
	if(argc == 3 && strcmp(argv[1], "sceneinfo") == 0) {
		printAll();
	}
	else if(argc == 7 && strcmp(argv[1], "pixelray") == 0) {
		pixelray(&myCamera, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
	}
	else if(argc == 7 && strcmp(argv[1], "firsthit") == 0) {
		//firsthit(&myCamera, &myObject, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
	}
	else if(strcmp(argv[1], "render") == 0) {
		mode = 0;
		if(argc == 6) {
			if(strcmp(argv[5], "-altbrdf") == 0) {
				mode = 1;
			}
		}
		render(myCamera, myLight, myObject, atoi(argv[3]), atoi(argv[4]), mode, 0); 
	}
	
	else if(strcmp(argv[1], "pixelcolor") == 0) {
		 pixelColor(myCamera, myLight, myObject, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 0, 1); 
	}

	else {
		printf("Usage: ./a.out sceneinfo <input_filename>\n");
		printf("Usage: ./a.out pixelray <input_filename> <width> <height> <x> <y>\n");
	}
	return 1;
}
