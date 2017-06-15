#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <iomanip>

//Globals
using namespace std;
using namespace glm;

Camera myCamera;
vector<Light> myLight;
vector<Object *> myObject;

const char *delim = "	 <>,{}";

void getPlane(FILE *fp, Plane *newPlane) {
	int i;
	char buff[255];
	newPlane->model = mat4(1.0f);
	char *token;
	float x, y, z;
	newPlane->model = mat4(1.f);
	for(i = 0; i < 5; i++) {
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
			newPlane->color.x = x;
			newPlane->color.y = y;
			newPlane->color.z = z;
			if((x = atof(strtok(NULL, delim)))) {
				newPlane->filter = x;
			}
		}

		else if(strcmp(token, "finish") == 0) {
			newPlane->roughness = 0.7f;
			newPlane->metallic = 0.1f;
			newPlane->ior = 1.6f;

			while((token = strtok(NULL, delim))) {
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

		else if(strcmp(token, "translate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));

			newPlane->model = translate(mat4(1.f), vec3(x, y, z)) * newPlane->model;
		}

		else if(strcmp(token, "rotate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newPlane->model = rotate(mat4(1.f), radians(z), vec3(0, 0, 1)) * newPlane->model;
			newPlane->model = rotate(mat4(1.f), radians(y), vec3(0, 1, 0)) * newPlane->model;
			newPlane->model = rotate(mat4(1.f), radians(x), vec3(1, 0, 0)) * newPlane->model;
		}

		else if(strcmp(token, "scale") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newPlane->model = scale(mat4(1.f), vec3(x, y, z)) * newPlane->model;
		}
	}
	newPlane->model = inverse(newPlane->model);
}

void getTriangle(FILE *fp, Triangle *newT) {
	int i;
	char buff[255];
	newT->model = mat4(1.0f);
	char *token;
	float x, y, z;
	for(i = 0; i < 9; i++) {
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
				newT->p1.x = x;
				newT->p1.y = y;
				newT->p1.z = z;
			}
			if(i == 1) {
				newT->p2.x = x;
				newT->p2.y = y;
				newT->p2.z = z;
			}
			if(i == 2) {
				newT->p3.x = x;
				newT->p3.y = y;
				newT->p3.z = z;
			}
		}

		else if(strcmp(token, "pigment") == 0) {
			strtok(NULL, delim);
			strtok(NULL, delim);
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newT->color.x = x;
			newT->color.y = y;
			newT->color.z = z;
			if((x = atof(strtok(NULL, delim)))) {
				newT->filter = x;
			}
		}

		else if(strcmp(token, "finish") == 0) {
			newT->roughness = 0.7f;
			newT->metallic = 0.1f;
			newT->ior = 1.6f;

			while((token = strtok(NULL, delim))) {
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

		else if(strcmp(token, "translate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));

			newT->model = translate(mat4(1.f), vec3(x, y, z)) * newT->model;
		}

		else if(strcmp(token, "rotate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newT->model = rotate(mat4(1.f), radians(z), vec3(0, 0, 1)) * newT->model;
			newT->model = rotate(mat4(1.f), radians(y), vec3(0, 1, 0)) * newT->model;
			newT->model = rotate(mat4(1.f), radians(x), vec3(1, 0, 0)) * newT->model;
		}

		else if(strcmp(token, "scale") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newT->model = scale(mat4(1.f), vec3(x, y, z)) * newT->model;
		}
	}
	newT->model = inverse(newT->model);
}

void getBox(FILE *fp, Box *newBox) {
	int i;
	char buff[255];
	char *token;
	float x, y, z;
	newBox->model = mat4(1.0f);
	
	for(i = 0; i < 8; i++) {
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
			newBox->color = vec3(x, y, z);
			if((x = atof(strtok(NULL, delim)))) {
				newBox->filter = x;
			}
		}
		else if(strcmp(token, "finish") == 0) {
			strtok(NULL,delim);
			x = atof(strtok(NULL, delim));
			strtok(NULL, delim);
			y = atof(strtok(NULL, delim));
			
			newBox->roughness = 0.7f;
			newBox->metallic = 0.1f;
			newBox->ior = 1.6f;

			while((token = strtok(NULL, delim))) {
				if(strcmp(token, "specular") == 0) {
					newBox->specular = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "roughness") == 0) {
					newBox->roughness = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "metallic") == 0) {
					newBox->metallic = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "ior") == 0) {
					newBox->ior = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "reflection") == 0) {
					newBox->reflection = atof(strtok(NULL, delim));
				}
				else if(strcmp(token, "refraction") == 0) {
					newBox->refraction = atof(strtok(NULL, delim));
				}
			}
			newBox->ambient = x;
			newBox->diffuse = y;
		}

		else if(strcmp(token, "translate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));

			newBox->model = translate(mat4(1.f), vec3(x, y, z)) * newBox->model;
		}

		else if(strcmp(token, "rotate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newBox->model = rotate(mat4(1.f), radians(z), vec3(0, 0, 1)) * newBox->model;
			newBox->model = rotate(mat4(1.f), radians(y), vec3(0, 1, 0)) * newBox->model;
			newBox->model = rotate(mat4(1.f), radians(x), vec3(1, 0, 0)) * newBox->model;
		}

		else if(strcmp(token, "scale") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newBox->model = scale(mat4(1.f), vec3(x, y, z)) * newBox->model;
		}
	}
	newBox->model = inverse(newBox->model);
}	

void getSphere(FILE *fp, Sphere *newSphere) {
	int i;
	char buff[255];
	char *token;
	float x, y, z;
	newSphere->model = mat4(1.0f);
	for(i = 0; i < 8; i++) {
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
			newSphere->color.x = x;
			newSphere->color.y = y;
			newSphere->color.z = z;
			if((x = atof(strtok(NULL, delim)))) {
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

			while((token = strtok(NULL, delim))) {
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
			newSphere->ambient = x;
			newSphere->diffuse = x;
		}

		else if(strcmp(token, "translate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));

			newSphere->model = translate(mat4(1.f), vec3(x, y, z)) * newSphere->model;
		}

		else if(strcmp(token, "rotate") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newSphere->model = rotate(mat4(1.f), radians(z), vec3(0, 0, 1)) * newSphere->model;
			newSphere->model = rotate(mat4(1.f), radians(y), vec3(0, 1, 0)) * newSphere->model;
			newSphere->model = rotate(mat4(1.f), radians(x), vec3(1, 0, 0)) * newSphere->model;
		}

		else if(strcmp(token, "scale") == 0) {
			x = y = z = 0;
			x = atof(strtok(NULL, delim));
			y = atof(strtok(NULL, delim));
			z = atof(strtok(NULL, delim));
			newSphere->model = scale(mat4(1.f), vec3(x, y, z)) * newSphere->model;
		}
	}
	newSphere->model = inverse(newSphere->model);
}

void getCamera(FILE *fp, int itr) {
	char buff[255];
	char *token;
	int i = 0;
	float x, y, z;
	for(i = 0; i < itr; i++) {
		fgets(buff, sizeof(buff), fp);
		if(strlen(buff) < 2) {
			break;
		}
	 	token = strtok(buff, delim);
		x = atof(strtok(NULL, delim));
		y = atof(strtok(NULL, delim));
		z = atof(strtok(NULL, delim));

		if(strcmp(token, "location") == 0) {
			myCamera.location.x = x;
			myCamera.location.y = y;
			myCamera.location.z = z;
		}
		else if(strcmp(token, "up") == 0) {
			myCamera.up.x = x;
			myCamera.up.y = y;
			myCamera.up.z = z;
		}
		else if(strcmp(token, "right") == 0) {
			myCamera.right.x = x;
			myCamera.right.y = y;
			myCamera.right.z = z;
		}
		else if(strcmp(token, "look_at") == 0) {
			myCamera.lookat.x = x;
			myCamera.lookat.y = y;
			myCamera.lookat.z = z;
		}
	}
}

void parse(char *filename) {
	FILE *fp;
	char buff[255];
    char *token;
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
				float tempx, tempy, tempz;
				token = strtok(NULL, delim);
				if(strcmp(token, "location") == 0) {
					tempx = atof(strtok(NULL, delim));
					tempy = atof(strtok(NULL, delim));
					tempz = atof(strtok(NULL, delim));
					myCamera.location = vec3(x, y, z);
					getCamera(fp, 3);
				}
				else {
					getCamera(fp, 4);
				}
			}

			else if(strcmp(token, "light_source") == 0) {
				Light newLight;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newLight.location = vec3(x, y, z);
				strtok(NULL, delim);
				strtok(NULL, delim);
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newLight.color = vec3(x, y, z);
				myLight.push_back(newLight);
			}

			else if(strcmp(token, "sphere") == 0) {
				Object *o = new Sphere;
				Sphere *newSphere = dynamic_cast<Sphere *>(o);
				newSphere->type = 1;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newSphere->center = vec3(x, y, z);
				x = atof(strtok(NULL, delim));
			   newSphere->radius = x;
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
				newPlane->normal = vec3(x, y, z);
				x = atof(strtok(NULL, delim));
				newPlane->distance = x;
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

			else if(strcmp(token, "box") == 0) {
				Object *o = new Box;
				Box *newBox = dynamic_cast<Box *> (o);
				newBox->type = 4;
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newBox->min = vec3(x, y, z);
				x = atof(strtok(NULL, delim));
				y = atof(strtok(NULL, delim));
				z = atof(strtok(NULL, delim));
				newBox->max = vec3(x, y, z);
				getBox(fp, newBox);
				myObject.push_back(newBox);
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
	printf("%lu light(s)\n\n", myLight.size());
	for(i = 0; i < (signed) myLight.size(); i++) {
		printf("Light[%d]:\n", i);
		cout << "- Location: {" <<  myLight.at(i).location.x << " " << myLight.at(i).location.y << " " <<  myLight.at(i).location.z << "}" << endl;
		cout << "- Color: {" <<  myLight.at(i).color.x << " " <<  myLight.at(i).color.y << " " <<  myLight.at(i).color.z << "}" << endl;
	}
	printf("\n---\n\n");
	printf("%lu object(s)\n", myObject.size());
	for(i = 0; i < (signed) myObject.size(); i++) {
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
		else if(myObject.at(i)->type == 4) {
			printf("- Type: Box\n");
			cout << "min.x " << dynamic_cast<Box *>(myObject.at(i))->min.x << " min.y " << dynamic_cast<Box *> (myObject.at(i))->min.y << " min.z " << dynamic_cast<Box *>(myObject.at(i))->min.z << endl;
			cout << "max.x " << dynamic_cast<Box *>(myObject.at(i))->max.x << " max.y " << dynamic_cast<Box *> (myObject.at(i))->max.y << " max.z " << dynamic_cast<Box *>(myObject.at(i))->max.z << endl;
		}
	}
}
