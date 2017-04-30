#ifndef PIXELRAY_H
#define PIXELRAY_H
#include <vector>
#include "data.h"

glm::vec3 pixelray (Camera *c, int width, int height, int x, int y, int mode);
glm::vec3 firsthit (Camera *c, std::vector<Object*> *o, int width, int height, int x, int y, int mode);

#endif
