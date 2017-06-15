#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include "parser.cpp"

int main(int argc, char *argv[]) {
	char *token;
	const char delimiter[2] = "=";
	parse(argv[2]);


	if(argc == 3 && strcmp(argv[1], "sceneinfo") == 0) {
		printAll();
	}
	else if(strcmp(argv[1], "render") == 0) {
		int blinn = 0;
		int fresnel = 0;
		int superS = 0;
		int sds = 0;
		int gi = 0;
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
				superS = atoi(token);
			}
			else if(strcmp(argv[temp + 5], "-sds") == 0) {
				sds = 1;
				cout << "Spacial Data Structures" << endl;
			}
			else if(strcmp(argv[temp + 5], "-gi") == 0) {
				gi = 1;
				cout << "Doing Monte Carlo" << endl;
			}
			temp--;
		}
		//render(myCamera, myLight, myObject, atoi(argv[3]), atoi(argv[4]), blinn, fresnel, superS, sds, gi, 0); 
	}
	
	else {
		printf("Usage: ./a.out sceneinfo <input_filename>\n");
		printf("Usage: ./a.out pixelray <input_filename> <width> <height> <x> <y>\n");
		printf("Usage: ./a.outrender <input_filename><width><height>\n");
	}

	return 1;
}
