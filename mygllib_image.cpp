#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "mygllib_image.h"

namespace mygl {

	Image* ReadImageFromFile(const std::string& path) {
		int _pic_type = 0;
		int _width = 0;
		int _height = 0;

		// 让返回的image_bits[0]是left-bottom.
		stbi_set_flip_vertically_on_load(true);
		unsigned char* bits = stbi_load(path.c_str(), &_width, &_height, &_pic_type, STBI_rgb_alpha);

		// bits是rgba 但是struct RGBA是bgra
		for (size_t i = 0; i < (_width * _height * 4); i+=4) {
			unsigned char r = bits[i]; // save r
			bits[i] = bits[i+2]; // b -> 0
			bits[i+2] = r;
		}

		Image* img = new Image(_width, _height, bits);
		stbi_image_free(bits);

		return img;
	}

}