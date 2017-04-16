#ifndef RENDER_H
#define RENDER_H
#include "data.h"
#include <vector>
void render(Camera c, std::vector<Light> l, std::vector<Object *> o, int width, int height);

#endif
