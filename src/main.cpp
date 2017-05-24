#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include "parser.cpp"
#include "render.h"
#include "pixelray.h"
#include "pixelcolor.h"

int main(int argc, char *argv[]) {
	int blinn = 0;
	int fresnel = 0;
	int superS = 0;
	char *token;
	const char delimiter[2] = "=";
	parse(argv[2]);


	if(argc == 3 && strcmp(argv[1], "sceneinfo") == 0) {
		printAll();
	}
	else if(argc == 7 && strcmp(argv[1], "pixelray") == 0) {
		pixelray(&myCamera, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
	}
	else if(argc == 7 && strcmp(argv[1], "firsthit") == 0) {
		cout << "Not Testable atm" << endl;
		//firsthit(&myCamera, &myObject, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
	}
	else if(strcmp(argv[1], "render") == 0) {
		blinn = 0;
		int temp = argc;
		temp = temp - 6;
		string s = (string) argv[temp + 5];
		while(temp >= 0) {
			if(strcmp(argv[temp + 5], "-altbrdf") == 0) {
				blinn = 1;
			}
			else if(strcmp(argv[temp + 5], "-fresnel") == 0) {
				fresnel = 1;
			}
			else if(s.substr(0, 4).compare("-ss=") == 0) {
				token = strtok(argv[temp + 5], delimiter);
				token = strtok(NULL, delimiter);
				//fprintf(stderr, "%s", token);
				superS = atoi(token);
			}
			temp--;
		}
		render(myCamera, myLight, myObject, atoi(argv[3]), atoi(argv[4]), blinn, fresnel, superS, 0); 
	}
	
	else if(strcmp(argv[1], "pixelcolor") == 0) {
		 cout << "Not Testable atm" << endl;
		 //pixelColor(myCamera, myLight, myObject, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 0, 1); 
	}
	/*
	else if(strcmp(argv[1], "pixeltrace") == 0) {
		int width = atoi(argv[3]);
		int height = atoi(argv[4]);
		int x = atoi(argv[5]);
		int y = atoi(argv[6]);
		if(strcmp(argv[7], "altbrdf") == 0) {
			mode = 1;
		}
		pixeltrace(myCamera, myLight, myObject, width, height, x, y, mode);
	}	*/

	else {
		printf("Usage: ./a.out sceneinfo <input_filename>\n");
		printf("Usage: ./a.out pixelray <input_filename> <width> <height> <x> <y>\n");
		printf("Usage: ./a.outrender <input_filename><width><height>\n");
	}

	return 1;
}
