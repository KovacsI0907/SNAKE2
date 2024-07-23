#include <object.h>
#include <geometry.h>
#include <sph.h>
#include <vectors.h>
#include <memory>

class SnakePiece : public Object {
	float CELL_SIZE = 2.0f/9;
public:
	int x = 0, y = 0;

	SnakePiece() : Object(std::make_unique<Sphere>(10), Material{ vec4(0,1,0,1), vec4(0.4,1,0.2,1), vec4(1,1,1,1), 5 }) {
		scale = vec3(1,1,1) * (CELL_SIZE / 2);
	}

	void moveTo(int x, int y) {
		this->x = x;
		this->y = y;
		this->position = vec3(x * CELL_SIZE, y * CELL_SIZE, 0);
	}
};