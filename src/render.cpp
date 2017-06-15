#include "data.h"
#include "pixelray.h"
#include "pixelcolor.h"
#include <vector>
#include "sds.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace glm;

void render(Camera c, vector<Light> l, vector<Object *> o, int width, int height, int brdf, int fresnel, int superS, int sds, int gi, int df, int mode) {
	const int numChannels = 3;
	const string fileName = "output.png";
	const ivec2 size = ivec2(width, height);
	bvh_node bn;
	bn.left = bn.right = NULL;

	unsigned char *data = new unsigned char[width * height * numChannels];
	
	if(sds) {
		recursiveTreeBuild(o, 0, &bn);
	}

	for(int y = 0; y  < height; y++) {
		for(int x = 0; x < width; x++) {
			vec3 pixRay, color;
			if(!superS) {
				pixRay = PixelRay(&c, width, height, x, y, 0);
				color = pixelColor(pixRay, c.location, l, o, width, height, x, y, brdf, mode, 0, &bn, sds, gi, 2, 128);
			}
			else if(superS) {
				for(int i = 0; i < superS; i++) {
					for(int j = 0; j < superS; j++) {
						pixRay = newpixelray(&c, width, height, x, y, 0, i, j, superS, df);
						color += pixelColor(pixRay, c.location, l, o, width, height, x, y, brdf, mode, 0, &bn, sds, gi, 2, 128);
					}
				}

				color = vec3(color.r / (superS * superS), color.g / (superS * superS), color.b / (superS * superS));
			}

			unsigned char red = std::min(255u, (unsigned int) round(color.r * 255.f));
			unsigned char green = std::min(255u, (unsigned int) round(color.g * 255.f));
			unsigned char blue = std::min(255u, (unsigned int) round(color.b * 255.f));

			data[(size.x * numChannels) * (size.y - 1 - y) + numChannels * x + 0] = red;
			data[(size.x * numChannels) * (size.y - 1 - y) + numChannels * x + 1] = green;
			data[(size.x * numChannels) * (size.y - 1 - y) + numChannels * x + 2] = blue;

		}
	}
	stbi_write_png(fileName.c_str(), size.x, size.y, numChannels, data, size.x * numChannels);
	delete[] data;
}
