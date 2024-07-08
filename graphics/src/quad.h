#include <geometry.h>
#include <vectors.h>
#include <vector>


class Quad : public Geometry {
public:
	Quad(vec3 lowerLeft, vec3 upperLeft, vec3 lowerRight, vec3 upperRight) {
		vec3 normal = cross(lowerLeft-upperLeft, lowerRight - upperLeft);

		vertices.push_back(lowerLeft);
		normals.push_back(normal);
		uvs.push_back(vec2(0, 0));

		vertices.push_back(upperLeft);
		normals.push_back(normal);
		uvs.push_back(vec2(0, 1));

		vertices.push_back(lowerRight);
		normals.push_back(normal);
		uvs.push_back(vec2(1,0));

		vertices.push_back(upperRight);
		normals.push_back(normal);
		uvs.push_back(vec2(1,1));
	}

	void draw() {
		Geometry::draw(GL_TRIANGLE_STRIP);
	}
};