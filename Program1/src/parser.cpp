#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.cpp"
#include <vector>

//Globals
using namespace std;
Camera myCamera;
vector<Light> myLight;
vector<Object *> myObject;

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
				Object *o = new Sphere;
				Sphere *newSphere = dynamic_cast<Sphere *>(o);
				newSphere->type = 1;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newSphere->set_center(x, y, z);
				x = atof(strtok(NULL, delim));
			   newSphere->set_radius(x);
				getSphere(fp, newSphere);
				myObject.push_back(newSphere);
			}

			else if(strcmp(token, "plane") == 0) {
				Object *o = new Plane;
				Plane *newPlane = dynamic_cast<Plane *> (o);
				newPlane->type = 2;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newPlane->set_normal(x, y, z);
				x = atof(strtok(NULL, delim));
				newPlane->set_distance(x);
				getPlane(fp, newPlane);
				myObject.push_back(newPlane);
			}
		}
	}
}

void printAll() {
	int i;
	printf("Camera:\n");
	cout << "- Location: {" << myCamera.location.x << " " <<  myCamera.location.y << " " << myCamera.location.z << "}" << endl;
	cout << "- Up: {" << myCamera.up.x << " " <<  myCamera.up.y << " " <<  myCamera.up.z << "}" << endl;
	cout << "- Right: {" << myCamera.right.x << " " << myCamera.right.y << " " << myCamera.right.z << "}" << endl;
	cout << "- Look at: {" <<  myCamera.lookat.x << " " << myCamera.lookat.y << " " << myCamera.lookat.z << "}" << endl;
	printf("\n---\n\n");
	printf("%d light(s)\n\n", myLight.size());
	for(i = 0; i < myLight.size(); i++) {
		printf("Light[%d]\n", i);
		cout << "- Locatoin: {" <<  myLight.at(i).location.x << " " << myLight.at(i).location.y << " " <<  myLight.at(i).location.z << "}" << endl;
		cout << "- Color: {" <<  myLight.at(i).color.x << " " <<  myLight.at(i).color.y << " " <<  myLight.at(i).color.z << "}" << endl;
	}
	printf("\n---\n\n");
	printf("%d objects(s)\n", myObject.size());
	for(i = 0; i < myObject.size(); i++) {
		printf("\nObject[%d]:\n", i);
		if(myObject.at(i)->type == 1) {
			printf("- Type: Sphere\n");
			cout << "- Center: {" << dynamic_cast<Sphere *>(myObject.at(i))->center.x << " " << dynamic_cast<Sphere*> (myObject.at(i))->center.y << " " << dynamic_cast<Sphere*>(myObject.at(i))->center.z << "}" << endl;
			cout << "- Radius: " <<  dynamic_cast<Sphere *>(myObject.at(i))->radius << endl;
			cout << "- Color: {" <<  dynamic_cast<Sphere *>(myObject.at(i))->color.x << " " << dynamic_cast<Sphere*> (myObject.at(i))->color.y << " " <<  dynamic_cast<Sphere *> (myObject.at(i))->color.z << "}" << endl;
	   	printf("- Material:\n");
			cout << "  - Ambient: " <<  dynamic_cast<Sphere *>(myObject.at(i))->ambient << endl;
			cout << "  - Diffuse: " <<  dynamic_cast<Sphere *>(myObject.at(i))->diffuse << endl;
		}
		else if(myObject.at(i)->type == 2) {
			printf("- Type: Plane\n");
			cout << "- Normal: {" << dynamic_cast<Plane *>(myObject.at(i))->normal.x << " " << dynamic_cast<Plane *> (myObject.at(i))->normal.y << " " << dynamic_cast<Plane *>(myObject.at(i))->normal.z << "}" << endl;
			cout << "- Distance: " << dynamic_cast<Plane *>(myObject.at(i))->distance << endl;
			cout << "- Color: {" <<  dynamic_cast<Plane *>(myObject.at(i))->color.x << " " << dynamic_cast<Plane*> (myObject.at(i))->color.y << " " <<  dynamic_cast<Plane *> (myObject.at(i))->color.z << "}" << endl;
			printf("- Material:\n");
			cout << "  - Ambient: " <<  dynamic_cast<Plane *>(myObject.at(i))->ambient << endl;
			cout << "  - Diffuse: " <<  dynamic_cast<Plane *>(myObject.at(i))->diffuse << endl;
		}
	}
}
