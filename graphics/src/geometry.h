#pragma once

#include <vector>
#include <vectors.h>
#include <GL/glew.h>
#include <exception>
#include <string>

class InvalidGeometryException : public std::exception {
	const char* message;

public:

	InvalidGeometryException(const char* msg) : message(msg) {};

	virtual const char* what() const throw() {
		return message;
	}
};

class Geometry {
protected:
	unsigned int vao;
	unsigned int vbos[3];
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;

public:
	Geometry() {}

	void create();

	void draw(GLenum mode);

	virtual void draw() = 0;

	~Geometry();
};