#pragma once

#include <vector>
#include <vectors.h>
#include <shaderprogram.h>
#include <image.h>

class Texture {
	unsigned int textureId;
	bool wasCreated = false;
protected:
	virtual const float* getPixels() = 0;
public:
	void create();
	bool created();
	void activate(ShaderProgram* shaderProgram);
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual ~Texture();
};

class ImageTexture : public Texture {
	std::shared_ptr<Image> image;
protected:
	const float* getPixels() override;
public:
	explicit ImageTexture(std::shared_ptr<Image> img) : image(img) {};
	int getWidth() override;
	int getHeight() override;
	~ImageTexture() override;
};