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
	return ScaleMatrix(scale)  * RotationMatrix(rotAxis, rotAngle)  * TranslateMatrix(position);
}

mat4 Object::ownModelInverse() {
	//TODO rotation matrix
	return TranslateMatrix(position * (-1.0f))  * RotationMatrix(rotAxis, -rotAngle)  * ScaleMatrix(scale * (1 / scale.length()));
}

void Object::setParent(Object* parent) {
	this->parent = parent;
}

void Object::setGeometry(std::unique_ptr<Geometry> geometry) {
	this->geometry = std::move(geometry);
	if (!this->geometry->created())
		this->geometry->create();
}

const std::vector<Object*> Object::getAllChildrenRecurse(){
	allChildrenRec.clear();
	getAllChildrenHelper(this);
	return allChildrenRec;
}

void Object::getAllChildrenHelper(Object* obj) {
	allChildrenRec.push_back(obj);

	for (Object* child : obj->children) {
		getAllChildrenHelper(child);
	}
}