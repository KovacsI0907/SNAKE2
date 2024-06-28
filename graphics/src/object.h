#pragma once

#include<material.h>
#include<geometry.h>
#include<vectors.h>
#include<vector>
#include<shaderprogram.h>

//Responsible for freeing geometries
class Object {
protected:

	Geometry* geometry = nullptr;
	Object* parent = nullptr;

	mat4 ownModel();
	mat4 ownModelInverse();
	
public:
	Material material;
	vec3 position = vec3(0, 0, 0);
	vec3 scale = vec3(1, 1, 1);
	float rotAngle = 0;
	vec3 rotAxis = vec3(0, 0, 1);

	Object() {};

	Object(Geometry* geometry, Material material) {
		this->geometry = geometry;
		if (!geometry->created())
			geometry->create();
		this->material = material;
	};

	void setGeometry(Geometry* geometry);

	void draw();

	mat4 getModel();

	mat4 getModelInverse();

	void setParent(Object* parent);
};