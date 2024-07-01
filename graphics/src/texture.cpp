#include <texture.h>
#include <GL/glew.h>
#include <shaderprogram.h>

void Texture::create() {
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, getPixels());

	// TODO make dynamic
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	wasCreated = true;
}

bool Texture::created() {
	return wasCreated;
}

void Texture::activate(ShaderProgram* shaderProgram) {
	if (!created()) {
		create();
	}

	// idk why
	int sampler = 0;
	
	shaderProgram->setUniform("sampler", sampler);

	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

Texture::~Texture() {
	glDeleteTextures(1, &textureId);
}