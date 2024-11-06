#pragma once

#include <vector>
#include <vectors.h>
#include <shaderprogram.h>

class Texture {
	unsigned int textureId;
	bool wasCreated = false;
protected:
	virtual float* getPixels() = 0;
public:
	void create();
	bool created();
	void activate(ShaderProgram* shaderProgram);
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual ~Texture();
};