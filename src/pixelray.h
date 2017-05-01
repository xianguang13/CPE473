#ifndef PIXELRAY_H
#define PIXELRAY_H
#include <vector>
#include "data.h"

glm::vec3 pixelray (Camera *c, int width, int height, int x, int y, int mode);
float firsthit (Camera *c, std::vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx);
float checkHit(glm::vec3 pixel, glm::vec3 origin, std::vector<Object*> *o, int width, int height, int x, int y, int mode, int *indx);

#endif
