#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.cpp"

int main(int argc, char *argv[]) {
	if(argc == 3 && strcmp(argv[1], "sceneinfo") == 0) {
		parse(argv[2]);
		printAll();
	}
	else {
		printf("Usage: ./a.out sceneinfo <input_filename>\n");
	}
	return 1;
}
