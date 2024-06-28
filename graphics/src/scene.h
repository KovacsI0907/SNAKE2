#pragma once

#include <vector>
#include <object.h>
#include <camera.h>
#include <light.h>

//Takes in dynamically allocated objects
//Responsible for freeing objects and lights
class Scene {
	std::vector<Object*> objects;
	void loadUniforms(Object* object);
	vec4 La = vec4(0.1, 0.1, 0.1, 0.1);

public:

	//TODO make getters/setters
	ShaderProgram* shaderProgram;
	Camera* camera;
	Light light;

	void addObject(Object* object);
	void deleteObject(Object* object);
	void drawAll();
	~Scene();
};