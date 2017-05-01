#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include "data.h"
#include "pixelray.h"

using namespace glm;
using namespace std;

bool checkShad(vec3 origin, vector<Object *> o, int width, int height, int x, int y, vec3 lightDir, float d) {
	int index; 
	float T = checkHit(lightDir, origin, &o, width, height, x, y, 0, &index);

	if(index == -1 | T > d)
		return true;
	else
		return false;
}

vec3 blinn_phong(Camera c, std::vector<Light> l, std::vector<Object *> o, float T, int i, int width, int height, int x, int y, vec3 pxRay) {
	vec3 color;
	//Solve point
	vec3 p = c.location + pxRay * T; 

	//Ambient
	color = o.at(i)->color * o.at(i)->ambient;

	//Calculate for all Lights
	for (unsigned int j = 0; j < l.size(); j++) {
		vec3 lightDir = normalize(l.at(j).location - p);
		vec3 norm = vec3(0, 0, 0);

		//CHECKU SHADOWU
		if(checkShad((p + lightDir * 0.1f), o, width, height, x, y, lightDir, glm::distance(l.at(j).location, p))) {
			//It's a Sphere
			if(o.at(i)->type == 1) {
				norm = normalize(p - dynamic_cast<Sphere*>(o.at(i))->center);
			}
			//It's a Plane
			else if(o.at(i)->type == 2) {
				norm = dynamic_cast<Plane*>(o.at(i))->normal;
			}

			//Diffuse
			color += o.at(i)->color * o.at(i)->diffuse * l.at(j).color * glm::max(0.f, dot(norm, lightDir));

			//Specular
			vec3 h = normalize(lightDir + normalize(-pxRay));
			float shininess = 2/pow(o.at(i)->roughness,2) - 2;
			color += o.at(i)->color * o.at(i)->specular * l.at(j).color * pow(glm::max(0.f, dot(h, norm)), shininess);
		}
	}
	return color;	
}	

vec3 cook_tor(Camera c, std::vector<Light> l, std::vector<Object *> o, float T, int i, int width, int height, int x, int y, vec3 pxRay) {
	vec3 color;
	//Solve point
	vec3 p = c.location + pxRay * T; 

	//Ambient
	color = o.at(i)->color * o.at(i)->ambient;

	//Calculate for all Lights
	for (unsigned int j = 0; j < l.size(); j++) {
		vec3 lightDir = normalize(l.at(j).location - p);
		vec3 norm = vec3(0, 0, 0);

		//CHECKU SHADOWU
		if(checkShad((p + lightDir * 0.1f), o, width, height, x, y, lightDir, glm::distance(l.at(j).location, p))) {
			//It's a Sphere
			if(o.at(i)->type == 1) {
				norm = normalize(p - dynamic_cast<Sphere*>(o.at(i))->center);
			}
			//It's a Plane
			else if(o.at(i)->type == 2) {
				norm = dynamic_cast<Plane*>(o.at(i))->normal;
			}

			vec3 h = normalize(lightDir + normalize(-pxRay));
			float shine = 2/pow(o.at(i)->roughness, 2) - 2;
			//D
			float D = (1/(M_PI * o.at(i)->roughness)) * pow(glm::max(0.f, dot(h, norm)), shine);

			//G
			float g1 = 2 * (dot(h, norm) * dot(norm, pxRay)) / dot(pxRay, h);
			float g2 = 2 * (dot(h, norm) * dot(norm, lightDir)) / dot(pxRay, h);
			float G = glm::min(g1, g2);
			G = glm::min(1.f, G);

			//F
			float f1 = pow(o.at(i)->ior - 1, 2) / pow(o.at(i)->ior+1, 2);
			float F = f1 + (1 - f1) * (1 - dot(pxRay, h));

			//RS
			float RS = (D * G * F) / (4 * dot(norm, lightDir) * dot(norm, pxRay));

			float d = 1 - o.at(i)->metallic;
			color += l.at(j).color * glm::max(0.f, dot(norm, lightDir)) * (d * (o.at(i)->color * o.at(i)->diffuse) + o.at(i)->metallic * RS);
			

		}
	}
	return color;	
}	



/** Pixel Color
 * Discription:
 *	Command casts a ray in the scene found in input_file and for pixel (x, y)
 *	finds first object it hits. It then computes the shaded color for the pixel
 *	indicated via BRDF.
 **/
vec3 pixelColor(Camera c, std::vector<Light> l, std::vector<Object *> o, int width, int height, int x, int y, int brdf, int mode) {
	int index;
	vec3 color = vec3(0, 0, 0);
	vec3 pixRay = pixelray(&c, width, height, x, y, mode);
	float T = firsthit (&c, &o, width, height, x, y, mode, &index);
	//Hits Something
	if(index != -1) {
		if(brdf == 0) {
			color = blinn_phong(c, l, o, T, index, width, height, x, y, pixRay);			
		}
		else if(brdf == 1) {
			color = cook_tor(c, l, o, T, index, width, height, x, y, pixRay);
		}
	}

	return color;
}

