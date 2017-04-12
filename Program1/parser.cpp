#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.cpp"
#include <vector>

//Globals
Camera myCamera;
std::vector<Light> myLight;
std::vector<Sphere> mySphere;
std::vector<Plane> myPlane;
const char *delim = "	 <>,{}";

void getPlane(FILE *fp, Plane *newPlane) {
	int i;
	char buff[255];
	char *token;
	float x, y, z;
	for(i = 0; i < 2; i++) {
		fgets(buff, sizeof(buff), fp);
		if(strlen(buff) < 2) {
			break;
		}
		token = strtok(buff, delim);
		if(strcmp(token, "pigment") == 0) {
			strtok(NULL, delim);
			strtok(NULL, delim);
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newPlane->set_color(x, y, z);
		}
		else if(strcmp(token, "finish") == 0) {
			strtok(NULL, delim);
			x = atof(strtok(NULL, delim));
			strtok(NULL, delim);
			y = atof(strtok(NULL, delim));
			newPlane->set_ambient(x);
			newPlane->set_diffuse(y);
		}
	}
}

void getSphere(FILE *fp, Sphere *newSphere) {
	int i;
	char buff[255];
	char *token;
	float x, y, z;
	for(i = 0; i < 3; i++) {
		fgets(buff, sizeof(buff), fp);
		if(strlen(buff) < 2) {
			break;
		}
		token = strtok(buff, delim);
		if(strcmp(token, "pigment") == 0) {
			strtok(NULL, delim);
			strtok(NULL, delim);
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newSphere->set_color(x, y, z);
		}
		else if(strcmp(token, "finish") == 0) {
			strtok(NULL,delim);
			x = atof(strtok(NULL, delim));
			strtok(NULL, delim);
			y = atof(strtok(NULL, delim));
			newSphere->set_ambient(x);
			newSphere->set_diffuse(y);
		}
		else if(strcmp(token, "translate") == 0) {
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newSphere->set_translate(x, y, z);
		}
	}
}

void getCamera(FILE *fp) {
	char buff[255];
	char *token;
	int i = 0;
	float x, y, z;
	for(i = 0; i < 4; i++) {
		fgets(buff, sizeof(buff), fp);
		if(strlen(buff) < 2) {
			break;
		}
	 	token = strtok(buff, delim);
		x = atof(strtok(NULL, delim));
		y = atof(strtok(NULL, delim));
		z = atof(strtok(NULL, delim));
		if(strcmp(token, "location") == 0) {
			myCamera.set_location(x, y, z);
		}
		else if(strcmp(token, "up") == 0) {
			myCamera.set_up(x, y, z);
		}
		else if(strcmp(token, "right") == 0) {
			myCamera.set_right(x, y, z);
		}
		else if(strcmp(token, "look_at") == 0) {
			myCamera.set_lookat(x, y, z);
		}
	}
}

void parse(char *filename) {
	FILE *fp;
	char buff[255];
   char *token;
	int i;
	float x, y, z;

	fp = fopen(filename, "r");

	if(fp == NULL) {
		perror("Error Opening File\n");
		exit(EXIT_FAILURE);
	}
	
	while(fgets(buff, sizeof(buff), fp)) {
		//Its a comment Ignore
		if(buff[0] == '/') {
			continue;
		}
		//Not a comment
		else {
			token = strtok(buff, delim);
			if(strcmp(token, "camera") == 0) {
				getCamera(fp);
			}

			else if(strcmp(token, "light_source") == 0) {
				Light newLight;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newLight.set_location(x, y, z);
				strtok(NULL, delim);
				strtok(NULL, delim);
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newLight.set_color(x, y, z);
				myLight.push_back(newLight);
			}

			else if(strcmp(token, "sphere") == 0) {
				Sphere newSphere;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newSphere.set_center(x, y, z);
				x = atof(strtok(NULL, delim));

				getSphere(fp, &newSphere);
				mySphere.push_back(newSphere);
			}

			else if(strcmp(token, "plane") == 0) {
				Plane newPlane;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newPlane.set_normal(x, y, z);
				x = atof(strtok(NULL, delim));
				newPlane.set_distance(x);
				getPlane(fp, &newPlane);
				myPlane.push_back(newPlane);
			}
		}
	}
}

void printAll() {
	int i;
	printf("Camera\n");
	printf("Location: %f, %f %f\n", myCamera.location.x, myCamera.location.y, myCamera.location.z);
	printf("Up: %f, %f, %f\n", myCamera.up.x, myCamera.up.y, myCamera.up.z);
	printf("Right: %f, %f, %f\n", myCamera.right.x, myCamera.right.y, myCamera.right.z);
	printf("Look at: %f, %f, %f\n", myCamera.lookat.x, myCamera.lookat.y, myCamera.lookat.z);
	for(i = 0; i < myLight.size(); i++) {
		printf("\nLight [%d]\n", i);
		printf("Locatoin: %f, %f, %f\n", myLight.at(i).location.x, myLight.at(i).location.y, myLight.at(i).location.z);
		printf("Color: %f, %f, %f\n", myLight.at(i).color.x, myLight.at(i).color.y, myLight.at(i).color.z);
	}
	for(i = 0; i < mySphere.size(); i++) {
		printf("\nSphere [%d]\n", i);
		printf("Center: %f, %f, %f\n", mySphere.at(i).center.x, mySphere.at(i).center.y, mySphere.at(i).center.z);
		printf("Radius: %f\n", mySphere.at(i).radius);
		printf("Color: %f, %f, %f\n", mySphere.at(i).color.x, mySphere.at(i).color.y, mySphere.at(i).color.z);
		printf("Ambient: %f\n", mySphere.at(i).ambient);
		printf("Diffuse: %f\n", mySphere.at(i).diffuse);
		printf("Translate: %f, %f, %f\n", mySphere.at(i).translate.x, mySphere.at(i).translate.y, mySphere.at(i).translate.z);
	}
	for(i = 0; i < myPlane.size(); i++) {
		printf("\nPlane [%d]\n", i);
		printf("Center: %f, %f, %f\n", myPlane.at(i).normal.x, myPlane.at(i).normal.y, myPlane.at(i).normal.z);
		printf("Distance: %f", myPlane.at(i).distance);
		printf("Color: %f, %f, %f\n", myPlane.at(i).color.x, myPlane.at(i).color.y, myPlane.at(i).color.z);
		printf("Ambient: %f\n", myPlane.at(i).ambient);
		printf("Diffuse: %f\n", myPlane.at(i).diffuse);
	}
}
