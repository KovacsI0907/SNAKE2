#pragma once

#include <vectors.h>
#include <geometry.h>
#include <math.h>
#include <GL/glew.h>

class Cylinder : public Geometry {
public:
	Cylinder(int resolution) {
		if (resolution <= 2) {
			throw InvalidGeometryException("Cylinder resolution can't be less than 3");
		}

		float rotAngle = 2.0f * M_PI / resolution;
		float uvStep = 1.0f / resolution;

		for (int i = 0; i < resolution + 1; i++) {
			vertices.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 0));
			vertices.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 1));
			normals.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 0));
			normals.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 0));
			uvs.push_back(vec2(uvStep * i, 0));
			uvs.push_back(vec2(uvStep * i, 1));
		}
	}

	virtual void draw() {
		Geometry::draw(GL_TRIANGLE_STRIP);
	}
};