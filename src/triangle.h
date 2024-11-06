#pragma once

#include <GL/glew.h>
#include <geometry.h>

class Triangle : public Geometry {
public:
	Triangle(vec3 a, vec3 b, vec3 c) {
		vertices.push_back(a);
		vertices.push_back(b);
		vertices.push_back(c);

		for (int i = 0; i < 3; i++) {
			normals.push_back(vec3(0, 0, 1));
			uvs.push_back(vec2(0, 0));
		}
	}

	void draw() {
		Geometry::draw(GL_TRIANGLES);
	}
};