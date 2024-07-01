#pragma once

#include <vector>
#include <object.h>
#include <camera.h>
#include <light.h>

//Takes in dynamically allocated objects
//Responsible for freeing objects and lights

/// <summary>
/// Stores objects and lights. Draws all objects and uploads necessary uniform variables.
/// TAKES OWNERSHIP of Objects and Textures added to it.
/// </summary>
class Scene {
	std::vector<Object*> objects;
	std::vector<Texture*> textures;
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
	/// Adds and takes ownership of the given texture.
	/// </summary>
	/// <param name="texture"></param>
	void addTexture(Texture* texture);

	/// <summary>
	/// Deletes and frees the given object.
	/// </summary>
	void deleteObject(Object* object);

	/// <summary>
	/// Deletes and frees the given texture.
	/// </summary>
	void deleteTexture(Texture* texture);

	/// <summary>
	/// Draws all added objects.
	/// </summary>
	void drawAll();

	~Scene();
};