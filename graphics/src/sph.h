#pragma once
#include <vectors.h>
#include <geometry.h>
#include <math.h>
#include <GL/glew.h>

/// <summary>
/// sphere
/// </summary>
class Sphere : public Geometry {
public:

	Sphere(int res) {
		if (res <= 4) {
			throw InvalidGeometryException("Sphere resolution can't be less than 5");
		}

		float radius = 1.0f, phi, theta;
		float uvStep = 1.0f / res;

		for (int i = 0; i < res; i++) {
			
			for (int j = 0; j < res+1; j++)
			{
				theta = 3.1415f * i / res;
				phi = 2 * 3.1415f * j / res;

				vertices.push_back(vec3(radius * sinf(theta) * cosf(phi), radius * sinf(theta) * sinf(phi), radius * cosf(theta)));

				normals.push_back(vec3(radius * sinf(theta) * cosf(phi), radius * sinf(theta) * sinf(phi), radius * cosf(theta)));
				uvs.push_back(vec2(uvStep * i, uvStep*j));

				theta = 3.1415f * (i+1) / res;

				vertices.push_back(vec3(radius * sinf(theta) * cosf(phi), radius * sinf(theta) * sinf(phi), radius * cosf(theta)));

				normals.push_back(vec3(radius * sinf(theta) * cosf(phi), radius * sinf(theta) * sinf(phi), radius * cosf(theta)));
				uvs.push_back(vec2(uvStep * (i+1), uvStep*j));
			}
		}
	}

	virtual void draw() {
		Geometry::draw(GL_TRIANGLE_STRIP);
	}
};