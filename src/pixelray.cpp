#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

using namespace glm;
using namespace std;

vec3 pixelray (Camera *c, int width, int height, int x, int y, int mode) {
	vec3 pixel;
	vec3 right = (-.5f + ((x + .5f) / (width))) * c->right;
	vec3 up = (-.5f + ((y + .5f) / (height))) * c->up;
	vec3 ray = -1.f * normalize(c->location - c->lookat);
	pixel = normalize(right + up + ray);
	if(mode == 1) {
		cout << std::setprecision(4);
		cout << "Pixel: [" << x << ", " << y << "] Ray: {" << c->location.x << " " << c->location.y << " " << c->location.z << "} -> {" << pixel.x << " " << pixel.y << " " << pixel.z << "}" << endl;
	}
	return pixel;
}

float firsthit (Camera *c, vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx) {
	vec3 pixel = pixelray(c, width, height, x, y, mode);
	int i, index;
	float A, B, C, temp, T = 99999999;
	vec3 K;
	for (i = 0; i < o->size(); i++) {
		if(o->at(i)->type == 1) {
			K = c->location - dynamic_cast<Sphere *>(o->at(i))->center;
			A = dot(pixel, pixel);
			B = dot((2.0f * pixel), K);
			C = dot(K, K) - (dynamic_cast<Sphere *>(o->at(i))->radius * dynamic_cast<Sphere *>(o->at(i))->radius);
			
			float desc =  B * B - 4 * A * C;
			if(desc >= 0) {
				temp =  (-(B) - sqrt(desc))/(2.0f * A);
				if(temp < T && temp >= 0) {
					T = temp;
					index = i;
				}
			}
			else {
				continue;
			}
		}
		else if(o->at(i)->type == 2) {
			Plane *ps = dynamic_cast<Plane *>(o->at(i));
			float denom = dot(pixel, ps->normal);
			if(denom < 1e-6) {
				temp = (ps->distance - dot(c->location, ps->normal))/ (dot(pixel, ps->normal));
				if(temp < T && temp >= 0) {
					T = temp;
					index = i;
				}
			}
			else {
				continue;
			}
		}
	}
	if(mode == 1) {
  		if(T != 99999999) {
			cout << "T = " <<  T << endl;
			if(o->at(index)->type == 1) {
				Sphere *s = dynamic_cast<Sphere *>(o->at(index));
				cout << "Object Type: Sphere" << endl;
				cout << "Color: (" << s->color.x << ", " << s->color.y << ", " << s->color.z << ")" << endl;
			}
			else {
				Plane *p = dynamic_cast<Plane *>(o->at(index));
				cout << "Object Type: Plane" << endl;
				cout << "Color: (" << p->color.x << ", " << p->color.y << ", " << p->color.z << ")" <<  endl;
			}
			*indx = index;
		}
		else {
			cout << "No Hit" << endl;
			*indx = -1;
		}
		return T;
	}

	else {
		if(T != 99999999) {
			*indx = index;
		}
		else {
			*indx = -1;
		}
		return T;
	}	
}


float checkHit(vec3 pixel, vec3 origin, vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx) {
	int i, index;
	float A, B, C, temp, T = 99999999;
	vec3 K;
	for (i = 0; i < o->size(); i++) {
		if(o->at(i)->type == 1) {
			K = origin - dynamic_cast<Sphere *>(o->at(i))->center;
			A = dot(pixel, pixel);
			B = dot((2.0f * pixel), K);
			C = dot(K, K) - (dynamic_cast<Sphere *>(o->at(i))->radius * dynamic_cast<Sphere *>(o->at(i))->radius);
			
			float desc =  B * B - 4 * A * C;
			if(desc >= 0) {
				temp =  (-(B) - sqrt(desc))/(2.0f * A);
				if(temp < T && temp >= 0) {
					T = temp;
					index = i;
				}
			}
			else {
				continue;
			}
		}
		else if(o->at(i)->type == 2) {
			Plane *ps = dynamic_cast<Plane *>(o->at(i));
			float denom = dot(pixel, ps->normal);
			if(denom < 1e-6) {
				temp = (ps->distance - dot(origin, ps->normal))/ (dot(pixel, ps->normal));
				if(temp < T && temp >= 0) {
					T = temp;
					index = i;
				}
			}
			else {
				continue;
			}
		}
	}
	if(mode == 1) {
  		if(T != 99999999) {
			cout << "T = " <<  T << endl;
			if(o->at(index)->type == 1) {
				Sphere *s = dynamic_cast<Sphere *>(o->at(index));
				cout << "Object Type: Sphere" << endl;
				cout << "Color: (" << s->color.x << ", " << s->color.y << ", " << s->color.z << ")" << endl;
			}
			else {
				Plane *p = dynamic_cast<Plane *>(o->at(index));
				cout << "Object Type: Plane" << endl;
				cout << "Color: (" << p->color.x << ", " << p->color.y << ", " << p->color.z << ")" <<  endl;
			}
			*indx = index;
		}
		else {
			cout << "No Hit" << endl;
			*indx = -1;
		}
		return T;
	}

	else {
		if(T != 99999999) {
			*indx = index;
		}
		else {
			*indx = -1;
		}
		return T;
	}	
}

