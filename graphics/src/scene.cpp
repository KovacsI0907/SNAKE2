#include <scene.h>
#include <texture.h>

void Scene::loadUniforms(Object* object) {
	mat4 M = object->getModel();
	mat4 MVP = M * camera->view() * camera->projection();
	mat4 Minv = object->getModelInverse();

	shaderProgram->setUniform("sampler", 0);

	if (object->material.texture) {
		shaderProgram->setUniform("hasTexture", true);
		object->material.texture->activate(shaderProgram);
	}
	else {
		shaderProgram->setUniform("hasTexture", false);
	}


	shaderProgram->setUniform("radAmbient", La);
	shaderProgram->setUniform("radLight", light.intensity);

	shaderProgram->setUniform("coeffAmbient", object->material.kAmbient);
	shaderProgram->setUniform("coeffDiffuse", object->material.kDiffuse);
	shaderProgram->setUniform("coeffSpecular", object->material.kSpecular);

	shaderProgram->setUniform("shine", object->material.shininess);

	shaderProgram->setUniform("wEyePos", camera->eye);
	shaderProgram->setUniform("wLightPos", light.position);

	shaderProgram->setUniform("MVP", MVP);
	shaderProgram->setUniform("M", M);
	shaderProgram->setUniform("Minv", Minv);
}

void Scene::addObject(Object* object) {
	// Check if the object pointer already exists in the vector
	auto it = std::find(objects.begin(), objects.end(), object);
	if (it == objects.end()) {
		objects.push_back(object);
	}
}

void Scene::addTexture(Texture* texture) {
	// Check if the texture pointer already exists in the vector
	auto it = std::find(textures.begin(), textures.end(), texture);
	if (it == textures.end()) {
		textures.push_back(texture);
		if (!texture->created()) {
			texture->create();
		}
	}
}

void Scene::deleteObject(Object* object) {
	auto it = std::find(objects.begin(), objects.end(), object);

	if (it != objects.end()) {
		objects.erase(it);
		delete object;
	}
}

void Scene::deleteTexture(Texture* texture) {
	auto it = std::find(textures.begin(), textures.end(), texture);

	if (it != textures.end()) {
		textures.erase(it);
		delete texture;
	}
}

void Scene::drawAll() {
	for (Object* object : objects) {
		loadUniforms(object);
		object->draw();
	}
}

Scene::~Scene() {
	for (Object* object : objects) {
		delete object;
	}

	for (Texture* texture : textures) {
		delete texture;
	}
}