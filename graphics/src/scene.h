#pragma once

#include <vector>
#include <object.h>
#include <camera.h>
#include <light.h>
#include <memory.h>

//Takes in dynamically allocated objects
//Responsible for freeing objects and lights

/// <summary>
/// Stores objects and lights. Draws all objects and uploads necessary uniform variables.
/// TAKES OWNERSHIP of Objects and Textures added to it.
/// </summary>
class Scene {
	std::vector<std::unique_ptr<Object>> objects;
	std::vector<std::unique_ptr<Texture>> textures;
	void loadUniforms(Object* object, Camera* camera, ShaderProgram* shaderProgram);

	int depthMapResolution = 1024;
	unsigned int depthMapFBO = 0;
	unsigned int depthMapTexture = 0;

	OrthographicCamera getDepthCam(float maxSceneRadius, vec4 lightPos);

public:
	Light light;
	Camera* camera;
	vec4 La;
	float radius = 5.0f;

	Scene(Camera* cam, Light light, vec4 ambientLight) : camera(cam), light(light), La(ambientLight) {};

	/// <summary>
	/// Needs to be called before render! Allocates objects for shadow rendering.
	/// </summary>
	void create();

	/// <summary>
	/// Adds and takes ownership of the given object.
	/// </summary>
	void addObject(std::unique_ptr<Object> object);

	/// <summary>
	/// Adds and takes ownership of the given texture.
	/// </summary>
	/// <param name="texture"></param>
	void addTexture(std::unique_ptr<Texture> texture);

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
	void renderScene(Camera* camera, ShaderProgram* shaderProgram, int winWidth, int winHeight);

	/// <summary>
	/// Renders the scene with shadows
	/// </summary>
	/// <param name="normalCamera">Camera used for rendering the scene</param>
	/// <param name="maxSceneRadius">Radius of the smallest sphere with an origin in (0,0,0) that fits all objects in the scene</param>
	/// <param name="normalShader">Shader used for rendering the scene</param>
	/// <param name="shadowShader">Shader used for rendering a depth map</param>
	/// <param name="windowWidth">Width of render area</param>
	/// <param name="windowHeight">Height of render area</param>
	void Scene::render(Camera* normalCamera, float maxSceneRadius, ShaderProgram* normalShader, ShaderProgram* shadowShader, int windowWidth, int windowHeight);

	~Scene() = default;

	const std::vector<Object*> getShadowCasters();
	const std::vector<Object*> getAllObjects();
};