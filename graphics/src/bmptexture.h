#pragma once
#include <texture.h>
#include <iostream>


class BmpTexture : public Texture{
protected:
	std::string path;
	float* getPixels() {

	}
public:
	BmpTexture(std::string& filePath) : path(filePath) {
	}
	int getWidth() = 0;
	int getHeight() = 0;
};