#ifndef PIXELCOLOR_H
#define PIXELCOLOR_H
#include "data.h"
#include <vector>

glm::vec3 pixelColor(Camera c, std::vector<Light> l, std::vector<Object *> o, int width, int height, int x, int y, int brdf, int mode);

#endif
