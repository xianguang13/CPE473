#include "data.h"
#include "pixelray.h"
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace glm;

void render(Camera c, vector<Light> l, vector<Object *> o, int width, int height) {
	const int numChannels = 3;
	const string fileName = "output.png";
	const ivec2 size = ivec2(640, 480);

	unsigned char *data = new unsigned char[width * height * numChannels];
	
	for(int y = 0; y  < height; y++) {
		for(int x = 0; x < width; x++) {
			glm::vec3 color = firsthit(&c, &o, width, height, x, y, 0);

			unsigned char red = (unsigned char) round(color.r * 255.f);
			unsigned char green = (unsigned char) round(color.g * 255.f);
			unsigned char blue = (unsigned char) round(color.b * 255.f);
			
			data[(size.x * numChannels) * (size.y - 1 - y) + numChannels * x + 0] = red;
			data[(size.x * numChannels) * (size.y - 1 - y) + numChannels * x + 1] = green;
			data[(size.x * numChannels) * (size.y - 1 - y) + numChannels * x + 2] = blue;

		}
	}

	stbi_write_png(fileName.c_str(), size.x, size.y, numChannels, data, size.x * numChannels);
	delete[] data;
}
