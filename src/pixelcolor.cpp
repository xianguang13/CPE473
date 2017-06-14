#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
#include <vector>
#include <limits>
#include "sds.h"
#include "data.h"
#include "pixelray.h"

using namespace glm;
using namespace std;

bool checkShad(vec3 origin, vector<Object *> o, int width, int height, int x, int y, vec3 lightDir, float d) {
	int index; 
	float T = numeric_limits<float>::max();
	checkHit(lightDir, origin, &o, width, height, x, y, 0, &index, &T);

	if(index == -1 || T > d)
		return true;
	else
		return false;
}

//Gets the Normal of the Object
vec3 getNormal(Object *o, vec3 p) {
	vec3 norm = vec3(0, 0, 0);
	mat4 n = transpose(o->model);

	//It's a Sphere
	if(o->type == 1) {
		norm = normalize(p - dynamic_cast<Sphere*>(o)->center);
	}
	//It's a Plane
	else if(o->type == 2) {
		norm = dynamic_cast<Plane*>(o)->normal;
	}
	//It's a Triangle		
	else if(o->type == 3) {
		Triangle *tempT = dynamic_cast<Triangle *>(o);
		vec3 P1 = tempT->p3 - tempT->p1;
		vec3 P2 = tempT->p2 - tempT->p1;
		norm = normalize(cross(P2, P1));
	}

	//It's a Box
	else if(o->type == 4) {
		Box *b = dynamic_cast<Box *>(o);
		if(epsilonEqual(p.x, b->min.x, 0.0001f))
			norm = vec3(-1, 0, 0);
		else if(epsilonEqual(p.x, b->max.x, 0.0001f))
			norm = vec3(1, 0, 0);
		else if(epsilonEqual(p.y, b->min.y, 0.0001f))
			norm = vec3(0, -1, 0);
		else if(epsilonEqual(p.y, b->max.y, 0.0001f))
			norm = vec3(0, 1, 0);
		else if(epsilonEqual(p.z, b->min.z, 0.0001f))
			norm = vec3(0, 0, -1);
		else if(epsilonEqual(p.z, b->max.z, 0.0001f))
			norm = vec3(0, 0, 1);
	}
	
	return n * vec4(norm.x, norm.y, norm.z, 0.0);
}

vec3 blinn_phong(vec3 c, std::vector<Light> l, std::vector<Object *> o,
float T, int i, int width, int height, int x, int y, vec3 pxRay, vec3 n, vec3 *color) {

	//Solve point
	vec3 p = c + pxRay * T; 
	pxRay = o.at(i)->model * vec4(pxRay.x, pxRay.y, pxRay.z, 0.0);
	
	//Ambient
	//color = o.at(i)->color * o.at(i)->ambient;

	//Calculate for all Lights
	for (unsigned int j = 0; j < l.size(); j++) {
		vec3 lightDir = normalize(l.at(j).location - p);

		//CHECKU SHADOWU
		if(checkShad((p + lightDir * 0.1f), o, width, height, x, y, lightDir, glm::distance(l.at(j).location, p))) {

			//Diffuse
			*color += o.at(i)->color * o.at(i)->diffuse * l.at(j).color * glm::max(0.f, dot(n, lightDir));

			//Specular
			vec3 h = normalize(lightDir + normalize(-pxRay));
			float shininess = 2/pow(o.at(i)->roughness,2) - 2;
			*color += o.at(i)->color * o.at(i)->specular * l.at(j).color * pow(glm::max(0.f, dot(h, n)), shininess);
		}
	}
	return *color;	
}	

vec3 cook_tor(vec3 c, std::vector<Light> l, std::vector<Object *> o, float T, int i, int width, int height, int x, int y, vec3 pxRay) {
	vec3 color;
	//Solve point
	vec3 p = c + pxRay * T; 

	//Ambient
	color = o.at(i)->color * o.at(i)->ambient;

	//Calculate for all Lights
	for (unsigned int j = 0; j < l.size(); j++) {
		vec3 lightDir = normalize(l.at(j).location - p);
		vec3 norm = vec3(0, 0, 0);

		//CHECKU SHADOWU
		if(checkShad((p + lightDir * 0.1f), o, width, height, x, y, lightDir, glm::distance(l.at(j).location, p))) {
			norm = getNormal(o.at(i), p);


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

float shlicksFormula(float ior, vec3 l, vec3 v) {
	float f1 = pow(ior - 1, 2) / pow(ior + 1, 2);
	float F = f1 + (1 - f1) * pow((1 - dot(-l, v)), 5);
	return F;
}

vec3 getRefractVector(float sneil_ratio, vec3 dir, vec3 norm) {
	return (sneil_ratio) * (dir - dot(dir, norm) * norm) - norm * sqrt(1 - pow(sneil_ratio,2) * (1 - pow(dot(-dir, norm), 2)));	
}

void printRecurse(vec3 dir, Object *o, int i, float t, vec3 n, vec3 p) {
   cout << setprecision(4);
   cout << "Ray: " << dir.x << " " << dir.y << " " << dir.z << endl;
   cout << "Hit Obj ID: (" << i << " - " << o->type << " at T = " << t << ", Int: {" << p.x << " " << p.y << " " << p.z << "}" << endl;
   cout << "Normal: " << n.x << " " << n.y << " " << n.z << endl;
   cout << endl << endl;
}

vec3 generateSamplePoints() {
	float u = rand() / (float) RAND_MAX;
	float v = rand() / (float) RAND_MAX;

	float radial = sqrt(u);
	float theta = 2.0 * M_PI * v;
	float x = radial * cos(theta);
	float y = radial * sin(theta);

	return vec3(x, y, sqrt(1 - u));
}

vec3 alignSampleVector(vec3 pt, vec3 up, vec3 normal) {
	float angle = acos(dot(up, normal));
	vec3 axis;
	if(up != normal)
		axis = cross(up, normal);
	else
		axis = normal;

	mat4 m = rotate(mat4(1.f), angle, axis);

	return vec3(m * vec4(pt, 0.0));
}

/** Pixel Color
 * Discription:
 *	Command casts a ray in the scene found in input_file and for pixel (x, y)
 *	finds first object it hits. It then computes the shaded color for the pixel
 *	indicated via BRDF.
 **/
vec3 pixelColor(vec3 dir, vec3 origin, std::vector<Light> l,
std::vector<Object *> o, int width, int height, int x,
int y, int brdf, int mode, int iterations, bvh_node *bn,
int flagSDS, int gi, int bounces, int N) {

	int index;
	vec3 color = vec3(0, 0, 0);
	float local_contribution = 0;
	float reflection_contribution = 0;
	float transmission_contribution = 0;
	//vec3 pixRay = pixelray(&c, width, height, x, y, 0);
	float T;
	Object *hitObj;
	vec3 reflection_color;
	vec3 refraction_color;
	float sneil_ratio;
	vector<Object *> closest;

	if(flagSDS) {
		testHit(dir, origin, bn, &closest);
		T = numeric_limits<float>::max();
		hitObj = checkHit(dir, origin, &closest, width, height, x, y, mode, &index, &T);
	}
	else
		hitObj = checkHit(dir, origin, &o, width, height, x, y, mode, &index, &T);

	//Hits Something
	if(index != -1 && hitObj->type < 5) {
		// Doe some basic setup for variables
		sneil_ratio = 1.0f/hitObj->ior;
		vec3 p = origin + dir * T;
		vec3 p_trans = hitObj->model * vec4(origin.x, origin.y, origin.z, 1.0) + hitObj->model * vec4(dir.x, dir.y, dir.z, 0.0) * T;
		vec3 norm = normalize(getNormal(hitObj, p_trans));

		if(gi) {
			vec3 amb = vec3(0, 0, 0);
			for(int i = 0; i < N; i++) {
				if(bounces > 1) {
					vec3 pt = generateSamplePoints();
					dir = alignSampleVector(pt, vec3(0, 0, 1), norm);
					amb += pixelColor(dir, (p+dir * 0.001f), l, o, width, height, x, y, brdf, mode, iterations, bn, flagSDS, gi, bounces - 1, N/8) * dot(dir, norm);
				}
			}

			color = hitObj->color * 2.0f / (float) N * amb;
		}

		else {
			color = hitObj->color * hitObj->ambient;
		}

		//Checks What method to use
		if(brdf == 0) {
			blinn_phong(origin, l, o, T, index, width, height, x, y, dir, norm, &color);
		}
		else if(brdf == 1) {
			//cook_tor(origin, l, o, T, index, width, height, x, y, dir, &color);
		}

		if (mode == 1)
			printRecurse(dir, hitObj, index, T, norm, p);

		//Check Number of Times
		//Reflection
		if(hitObj->reflection > 0 && iterations < 6) {
			vec3 reflection_vector = dir - 2 * (dot(dir, norm)) * norm;
			reflection_color = pixelColor(reflection_vector, (p+norm*.001f), l, o, width, height, x, y, brdf, mode, iterations + 1, bn, flagSDS, gi, bounces, N);
		}

		//Refraction
		if(hitObj->refraction >= 0 && iterations < 6) {
			float dist = 0;
			if(dot(dir, norm) > 0) {
				norm = -norm;
				sneil_ratio = hitObj->ior;
				dist = T;
			}

		
			vec3 refraction_vector = (sneil_ratio) * (dir - dot(dir, norm) * norm) - norm * sqrt(1 - pow(sneil_ratio,2) * (1 - pow(dot(dir, -norm), 2)));	

			//Beers Law
			vec3 absorbance = (1.f - hitObj->color) * .15f * -dist;
			vec3 attenuation = vec3(pow(M_E, absorbance.r), pow(M_E, absorbance.g), pow(M_E, absorbance.b));

			refraction_color = pixelColor(refraction_vector, (p - norm *.001f), l, o, width, height, x, y, brdf, mode, iterations + 1, bn, flagSDS, gi, bounces, N) * attenuation;
		}

		float fresnel_reflectance = shlicksFormula(hitObj->ior, dir, norm);
		local_contribution = (1 - hitObj->filter) * (1 - hitObj->reflection);
		reflection_contribution = (1 - hitObj->filter) * (hitObj->reflection) + (hitObj->filter) * (fresnel_reflectance);
		transmission_contribution = (hitObj->filter) * (1 - fresnel_reflectance);
	}

	//If printing mode is on
	if(mode == 1) {
		cout << std::setprecision(4);
		cout << "Color: (" << (unsigned int) round(255.f * color.r) << ", " << (unsigned int) round(255.f * color.g) << ", " << (unsigned int) round(255.f * color.b) << ")" << endl;
	}
	
	return local_contribution * color + reflection_color * reflection_contribution + refraction_color * transmission_contribution;
}


