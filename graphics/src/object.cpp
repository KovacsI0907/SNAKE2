#include <object.h>

void Object::draw() {
	if (geometry != nullptr) {
		geometry->draw();
	}
}

mat4 Object::getModel() {
	if (!parent) {
		return ownModel();
	}

	return parent->getModel() * ownModel();
}

mat4 Object::getModelInverse() {
	if (!parent) {
		return ownModelInverse();
	}

	return ownModelInverse() * parent->getModelInverse();
}

mat4 Object::ownModel() {
	//TODO rotation matrix
	return ScaleMatrix(scale) /* * RotationMatrix(rotAxis, rotAngle) */ * TranslateMatrix(position);
}

mat4 Object::ownModelInverse() {
	//TODO rotation matrix
	return TranslateMatrix(position * (-1.0f)) /* * RotationMatrix(rotAxis, -rotAngle) */ * ScaleMatrix(scale * (1 / scale.length()));
}

void Object::setParent(Object* parent) {
	this->parent = parent;
}

void Object::setGeometry(Geometry* geometry) {
	if (this->geometry) {
		delete geometry;
	}

	this->geometry = geometry;
	if (!geometry->created())
		geometry->create();
}