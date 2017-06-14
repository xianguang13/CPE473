#ifndef PIXELRAY_H
#define PIXELRAY_H
#include <vector>
#include "data.h"

glm::vec3 PixelRay (Camera *c, int width, int height, int x, int y, int mode);
glm::vec3 newpixelray (Camera *c, int width, int height, int x, int y, int mode, int i, int j, int superS);
float firsthit (Camera *c, std::vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx);
Object* checkHit(glm::vec3 pixel, glm::vec3 origin, std::vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx, float *T);
void boxFirstHit(glm::vec3 origin, Box *myB, glm::vec3 pixel, float *t, int ind, int *index);

#endif
