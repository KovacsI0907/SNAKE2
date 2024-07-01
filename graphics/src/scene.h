#pragma once

#include <vector>
#include <object.h>
#include <camera.h>
#include <light.h>

//Takes in dynamically allocated objects
//Responsible for freeing objects and lights

/// <summary>
/// Stores objects and lights. Draws all objects and uploads necessary uniform variables.
/// TAKES OWNERSHIP of Objects added to it.
/// </summary>
class Scene {
	std::vector<Object*> objects;
	void loadUniforms(Object* object);
	vec4 La = vec4(0.1, 0.1, 0.1, 0.1);

public:

	//TODO make getters/setters
	ShaderProgram* shaderProgram;
	Camera* camera;
	Light light;

	/// <summary>
	/// Adds and takes ownership of the given object.
	/// </summary>
	void addObject(Object* object);

	/// <summary>
	/// Deletes and frees the given object.
	/// </summary>
	void deleteObject(Object* object);

	/// <summary>
	/// Draws all added objects.
	/// </summary>
	void drawAll();

	~Scene();
};