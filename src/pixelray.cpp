#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

using namespace glm;
using namespace std;

vec3 PixelRay (Camera *c, int width, int height, int x, int y, int mode) {
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

vec3 newpixelray (Camera *c, int width, int height, int x, int y, int mode, int i, int j, int superS) {
	vec3 pixel;
	vec3 right = (-.5f + ((x + (j + .5f) / superS) / (width))) * c->right;
	vec3 up = (-.5f + ((y + (i + .5f) / superS) / (height))) * c->up;
	vec3 ray = -1.f * normalize(c->location - c->lookat);
	pixel = normalize(right + up + ray);
	if(mode == 1) {
		cout << std::setprecision(4);
		cout << "Pixel: [" << x << ", " << y << "] Ray: {" << c->location.x << " " << c->location.y << " " << c->location.z << "} -> {" << pixel.x << " " << pixel.y << " " << pixel.z << "}" << endl;
	}
	return pixel;
}

int hit_tri(vec3 r, vec3 camera, vec3 a, vec3 b, vec3 c, float *T) {
	//compute temp
	float A = a.x - b.x;
	float B = a.y - b.y;
	float C = a.z - b.z;

	float D = a.x - c.x;
	float E = a.y - c.y;
	float F = a.z - c.z;

	float G = r.x;
	float H = r.y;
	float I = r.z;

	float J = a.x - camera.x;
	float K = a.y - camera.y;
	float L = a.z - camera.z;

	float M = A * (E*I - H*F) + B * (G*F - D*I) + C*(D*H - E*G);
	float Beta = (J* (E * I - H * F)+K*(G*F-D*I)+L*(D*H-E*G)) / M;
	float Gamma = (I*(A*K - J*B) + H*(J*C - A*L) + G*(B*L - K*C)) / M;
	float Temp = -(F*(A*K - J*B) + E*(J*C - A*L) + D*(B*L - K*C)) / M;

	if(Gamma < 0 || Gamma > 1) {
		return 0;
	}
	if(Beta < 0 || Beta > 1 - Gamma) {
		return 0;
	}
	if(Temp < *T && Temp >= 0) {
		*T = Temp;
		return 1;
	}
	return 0;
}	

void swap(float *x, float *y) {
	float tmp = *x;
	*x = *y;
	*y = tmp;
}

void helperSwap(float *t1, float *t2, float *tgmin, float *tgmax) {
	if(*t1 > *t2) {
		float temp = *t1;
		*t1 = *t2;
		*t2 = temp;
	}
	if(*t1 > *tgmin) {
		*tgmin = *t1;
	}
	if(*t2 < *tgmax) {
		*tgmax = *t2;
	}
}

void boxFirstHit(vec3 origin, Box *myB, vec3 pixel, float *t, int ind, int *index) {
	vec4 tempV = myB->model * vec4(pixel, 0.0);
	pixel = vec3(tempV.x, tempV.y, tempV.z);
	tempV = myB->model * vec4(origin, 1.0);
	origin = vec3(tempV.x, tempV.y, tempV.z);

	float t1, t2;
	float tgmin = -99999999;
	float tgmax = numeric_limits<float>::max();

	if(pixel.x != 0) {
		if(pixel.x == 0 && (origin.x >= myB->min.x || origin.x <= myB->max.x))
			return;
		t1 = (myB->min.x - origin.x) / pixel.x;	
		t2 = (myB->max.x - origin.x) / pixel.x;
		helperSwap(&t1, &t2, &tgmin, &tgmax);
	}

	if(pixel.y != 0) {
		if(pixel.y == 0 && (origin.y >= myB->min.y || origin.y <= myB->max.y))
			return;
		t1 = (myB->min.y - origin.y) / pixel.y;
		t2 = (myB->max.y - origin.y) / pixel.y;
		helperSwap(&t1, &t2, &tgmin, &tgmax);
	}

	if(pixel.z != 0) {
		if(pixel.z == 0 && (origin.z >= myB->min.z || origin.z <= myB->max.z))
			return;
		t1 = (myB->min.z - origin.z) / pixel.z;
		t2 = (myB->max.z - origin.z) / pixel.z;
		helperSwap(&t1, &t2, &tgmin, &tgmax);
	}

	if(tgmin > tgmax || tgmax < 0)
		return;
	if(tgmin > 0 && tgmin < *t) {
		*t = tgmin;
		*index = ind;
	}
	else if(tgmax > 0 && tgmax < *t) {
		*t = tgmax;
		*index = ind;
	}
}

Object* checkHit(vec3 pixel, vec3 origin, vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx, float *T) {
	int i, index;
	float A, B, C, temp, temp2;
	*T = numeric_limits<float>::max();
	vec3 K, tpixel, torigin;
	index = -1;
	for (i = 0; i < o->size(); i++) {
		// Sphere First Hit
		if(o->at(i)->type == 1) {
			tpixel = o->at(i)->model * vec4(pixel.x, pixel.y, pixel.z, 0.0);
			torigin = o->at(i)->model * vec4(origin.x, origin.y, origin.z, 1.0);
			K = torigin - dynamic_cast<Sphere *>(o->at(i))->center;
			A = dot(tpixel, tpixel);
			B = 2 * dot(tpixel, K);
			C = dot(K, K) - (dynamic_cast<Sphere *>(o->at(i))->radius * dynamic_cast<Sphere *>(o->at(i))->radius);
			
			float desc =  B * B - 4 * A * C;
			if(desc >= 0) {
				temp =  (-(B) - sqrt(desc))/(2.0f * A);
				float temp2 = (-(B) + sqrt(desc))/(2.0f * A);
				

				if(temp < 0 && temp2 > 0) {
					temp = temp2;
				}

				if(temp < *T && temp > 0) {
					*T = temp;
					index = i;
				}
			}
			else {
				continue;
			}
		}
		// Plane First Hit
		else if(o->at(i)->type == 2) {
			Plane *ps = dynamic_cast<Plane *>(o->at(i));
			float denom = dot(pixel, ps->normal);
			temp = (ps->distance - dot(origin, ps->normal))/ (dot(pixel, ps->normal));
			if(temp < *T && temp >= 0) {
				*T = temp;
				index = i;
			}
		}
		// Triangle First Hit
		else if(o->at(i)->type == 3) {
			Triangle *myT = dynamic_cast<Triangle *>(o->at(i));
			if(hit_tri(pixel, origin, myT->p1, myT->p2, myT->p3, T)) {
				index = i;
			}
		}
		// Box First Hit
		else if(o->at(i)->type == 4) {
			Box *myB = dynamic_cast<Box *>(o->at(i));
			boxFirstHit(origin, myB, pixel, T, i, &index);
		}
	}

	if(*T != numeric_limits<float>::max()) {
		*indx = index;
	}
	else {
		*indx = -1;
		Object *nullObj = new Object;
		nullObj->type = 1000;
		return nullObj;
	}
	return o->at(index);
}

