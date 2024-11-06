#pragma once

#include <vector>
#include <vectors.h>
#include <GL/glew.h>
#include <exception>
#include <string>

class InvalidGeometryException : public std::exception {
	const char* message;

public:
	/// <param name="msg">Message that should be returned by what()</param>
	InvalidGeometryException(const char* msg) : message(msg) {};

	virtual const char* what() const throw() {
		return message;
	}
};

/// <summary>
/// Represents a geometry that has vertices, normals and uvs. Handles communication with openGL, but does not update the mesh when the geometry changes.
/// </summary>
class Geometry {
protected:
	unsigned int vao;
	unsigned int vbos[3];
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;
	bool wasCreated = false;

public:
	/// <summary>
	/// Constructor of the geometry. Vertices, normals and uvs should be defined here!
	/// </summary>
	Geometry() {}

	/// <summary>
	/// Creates vao and vbos and fills vbos with data.
	/// </summary>
	void create();

	/// <summary>
	/// Draws all vertices in the specified mode.
	/// </summary>
	/// <param name="mode"></param>
	void draw(GLenum mode);

	/// <summary>
	/// Draws the shape.
	/// </summary>
	virtual void draw() = 0;

	/// <summary>
	/// Returns wether the vao and vbos have been initialized already
	/// </summary>
	/// <returns></returns>
	bool created() {
		return wasCreated;
	}

	~Geometry();
};