#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.cpp"
#include <vector>
#include <iostream>
#include <iomanip>

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
			if(x = atof(strtok(NULL, delim))) {
				newPlane->filter = x;
			}
		}
		else if(strcmp(token, "finish") == 0) {
			newPlane->roughness = 0.7f;
			newPlane->metallic = 0.1f;
			newPlane->ior = 1.6f;

			while(token = strtok(NULL, delim)) {
				if(strcmp(token, "ambient") == 0) {
					newPlane->ambient = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "diffuse") == 0) {
					newPlane->diffuse = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "specular") == 0) {
					newPlane->specular = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "roughness") == 0) {
					newPlane->roughness = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "metallic") == 0) {
					newPlane->metallic = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "ior") == 0) {
					newPlane->ior = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "reflection") == 0) {
					newPlane->reflection = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "refraction") == 0) {
					newPlane->refraction = atof(strtok(NULL, delim));
				}
			}
		}
	}
}

void getTriangle(FILE *fp, Triangle *newT) {
	int i;
	char buff[255];
	char *token;
	float x, y, z;
	for(i = 0; i < 6; i++) {
		fgets(buff, sizeof(buff), fp);
		if(strlen(buff) < 2) {
			break;
		}
		token = strtok(buff, delim);
		if(i < 3) {
			x = atof(token);
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			if(i == 0) {
				newT->set_p1(x, y, z);
			}
			if(i == 1) {
				newT->set_p2(x, y, z);
			}
			if(i == 2) {
				newT->set_p3(x, y, z);
			}
		}

		else if(strcmp(token, "pigment") == 0) {
			strtok(NULL, delim);
			strtok(NULL, delim);
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newT->set_color(x, y, z);
			if(x = atof(strtok(NULL, delim))) {
				newT->filter = x;
			}
		}

		else if(strcmp(token, "finish") == 0) {
			newT->roughness = 0.7f;
			newT->metallic = 0.1f;
			newT->ior = 1.6f;

			while(token = strtok(NULL, delim)) {
				if(strcmp(token, "ambient") == 0) {
					newT->ambient = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "diffuse") == 0) {
					newT->diffuse = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "specular") == 0) {
					newT->specular = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "roughness") == 0) {
					newT->roughness = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "metallic") == 0) {
					newT->metallic = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "ior") == 0) {
					newT->ior = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "reflection") == 0) {
					newT->reflection = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "refraction") == 0) {
					newT->refraction = atof(strtok(NULL, delim));
				}
			}
		}
	}
}

void getSphere(FILE *fp, Sphere *newSphere) {
	int i, marker = 0;
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
			if(x = atof(strtok(NULL, delim))) {
				newSphere->filter = x;
			}
		}
		else if(strcmp(token, "finish") == 0) {
			strtok(NULL,delim);
			x = atof(strtok(NULL, delim));
			strtok(NULL, delim);
			y = atof(strtok(NULL, delim));
			
			newSphere->roughness = 0.7f;
			newSphere->metallic = 0.1f;
			newSphere->ior = 1.6f;

			while(token = strtok(NULL, delim)) {
				if(strcmp(token, "specular") == 0) {
					newSphere->specular = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "roughness") == 0) {
					newSphere->roughness = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "metallic") == 0) {
					newSphere->metallic = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "ior") == 0) {
					newSphere->ior = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "reflection") == 0) {
					newSphere->reflection = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "refraction") == 0) {
					newSphere->refraction = atof(strtok(NULL, delim));
				}
			}
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

			else if(strcmp(token, "triangle") == 0) {
				Object *o = new Triangle;
				Triangle *newTriang = dynamic_cast<Triangle *> (o);
				newTriang->type = 3;
				getTriangle(fp, newTriang);
				myObject.push_back(newTriang);
			}
		}
	}
}

void printAll() {
	int i;
	cout << std::setprecision(4);
	printf("Camera:\n");
	cout << "- Location: {" << myCamera.location.x << " " <<  myCamera.location.y << " " << myCamera.location.z << "}" << endl;
	cout << "- Up: {" << myCamera.up.x << " " <<  myCamera.up.y << " " <<  myCamera.up.z << "}" << endl;
	cout << "- Right: {" << myCamera.right.x << " " << myCamera.right.y << " " << myCamera.right.z << "}" << endl;
	cout << "- Look at: {" <<  myCamera.lookat.x << " " << myCamera.lookat.y << " " << myCamera.lookat.z << "}" << endl;
	printf("\n---\n\n");
	printf("%d light(s)\n\n", myLight.size());
	for(i = 0; i < myLight.size(); i++) {
		printf("Light[%d]:\n", i);
		cout << "- Location: {" <<  myLight.at(i).location.x << " " << myLight.at(i).location.y << " " <<  myLight.at(i).location.z << "}" << endl;
		cout << "- Color: {" <<  myLight.at(i).color.x << " " <<  myLight.at(i).color.y << " " <<  myLight.at(i).color.z << "}" << endl;
	}
	printf("\n---\n\n");
	printf("%d object(s)\n", myObject.size());
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
		else if(myObject.at(i)->type == 3) {
			printf("- Type: Triangle\n");
			cout << "- p1: {" << dynamic_cast<Triangle *>(myObject.at(i))->p1.x << " " << dynamic_cast<Triangle *> (myObject.at(i))->p1.y << " " << dynamic_cast<Triangle *>(myObject.at(i))->p1.z << "}" << endl;
			cout << "- p2: {" << dynamic_cast<Triangle *>(myObject.at(i))->p2.x << " " << dynamic_cast<Triangle *> (myObject.at(i))->p2.y << " " << dynamic_cast<Triangle *>(myObject.at(i))->p2.z << "}" << endl;
			cout << "- p3: {" << dynamic_cast<Triangle *>(myObject.at(i))->p3.x << " " << dynamic_cast<Triangle *> (myObject.at(i))->p3.y << " " << dynamic_cast<Triangle *>(myObject.at(i))->p3.z << "}" << endl;
			cout << "- Color: {" <<  dynamic_cast<Triangle *>(myObject.at(i))->color.x << " " << dynamic_cast<Triangle*> (myObject.at(i))->color.y << " " <<  dynamic_cast<Triangle *> (myObject.at(i))->color.z << "}" << endl;
			printf("- Material:\n");
			cout << "  - Ambient: " <<  dynamic_cast<Triangle *>(myObject.at(i))->ambient << endl;
			cout << "  - Diffuse: " <<  dynamic_cast<Triangle *>(myObject.at(i))->diffuse << endl;
		}
	}
}
