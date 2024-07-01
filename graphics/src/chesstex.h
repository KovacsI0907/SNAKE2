#pragma once

#include <texture.h>
#include <vectors.h>
#include <vector>

class ChessTex : public Texture {
	std::vector<vec4> pixels;

	float* getPixels() {
		return &pixels[0].x;
	}
public:
	ChessTex(vec4 color1, vec4 color2) {
		pixels.push_back(color1);
		pixels.push_back(color2);
		pixels.push_back(color2);
		pixels.push_back(color1);
	}

	int getWidth() {
		return 2;
	}

	int getHeight() {
		return 2;
	}
};