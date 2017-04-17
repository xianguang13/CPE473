#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.cpp"
#include "render.h"
#include "pixelray.h"

int main(int argc, char *argv[]) {
	parse(argv[2]);
	if(argc == 3 && strcmp(argv[1], "sceneinfo") == 0) {
		printAll();
	}
	else if(argc == 7 && strcmp(argv[1], "pixelray") == 0) {
		pixelray(&myCamera, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
	}
	else if(argc == 7 && strcmp(argv[1], "firsthit") == 0) {
		firsthit(&myCamera, &myObject, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
	}
	else if(argc == 5 && strcmp(argv[1], "render") == 0) {
		render(myCamera, myLight, myObject, atoi(argv[3]), atoi(argv[4])); 
	}

	else {
		printf("Usage: ./a.out sceneinfo <input_filename>\n");
		printf("Usage: ./a.out pixelray <input_filename> <width> <height> <x> <y>\n");
	}
	return 1;
}
