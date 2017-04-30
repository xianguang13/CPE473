#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

#include "data.h"
#include "pixelray.h"

using namespace glm
using namespace std

/** Pixel Color
 * Discription:
 *	Command casts a ray in the scene found in input_file and for pixel (x, y)
 *	finds first object it hits. It then computes the shaded color for the pixel
 *	indicated via BRDF.
 **/
void pixelcolor(Camera c, std::vector<Light> l, std::vector<Object *> o, int width, int height, int x, int y, int mode) {
	pixelray (&c, width, height, x, y, 1);
	firsthit (&c, &o, width, height, x, y, 1);



}

