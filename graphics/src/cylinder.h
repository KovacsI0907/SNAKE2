#pragma once

#include <vectors.h>
#include <geometry.h>
#include <math.h>
#include <GL/glew.h>


/// <summary>
/// Implementation of the Geometry class, that can be used to display a cylinder.
/// </summary>
/// <remarks>
/// Pivot point is at the origin. Which is the middle of the bottom of the cylinder. The cylinder has a height of 1 and a radius of 1 (up is +z).
/// </remarks>
class Cylinder : public Geometry {
public:

	/// <param name="resolution">The number of sides the cylinder will have.</param>
	Cylinder(int resolution) {
		if (resolution <= 2) {
			throw InvalidGeometryException("Cylinder resolution can't be less than 3");
		}

		// The cylinder consists of 2 circles which will be rendered as a triangle strip

		float rotAngle = 2.0f * M_PI / resolution;
		float uvStep = 1.0f / resolution;

		for (int i = 0; i < resolution + 1; i++) {

			// Add vertices at the current angle
			vertices.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 0));
			vertices.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 1));

			// Normals are the same as the lower vertex at this angle (since the cylinder is at the origin)
			normals.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 0));
			normals.push_back(vec3(cosf(i * rotAngle), sinf(i * rotAngle), 0));

			// UVs are stretched so that the surface of the cylinder lines up with the uv plane
			uvs.push_back(vec2(uvStep * i, 0));
			uvs.push_back(vec2(uvStep * i, 1));
		}
	}

	virtual void draw() {
		Geometry::draw(GL_TRIANGLE_STRIP);
	}
};