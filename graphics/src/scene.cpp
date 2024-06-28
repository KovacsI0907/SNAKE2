#include <scene.h>

void Scene::loadUniforms(Object* object) {
	mat4 M = object->getModel();
	mat4 MVP = M * camera->view() * camera->projection();
	mat4 Minv = object->getModelInverse();


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
	objects.push_back(object);
}

void Scene::deleteObject(Object* object) {
	auto it = find(objects.begin(), objects.end(), object);

	if (it != objects.end()) {
		objects.erase(it);
	}

	delete object;
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
}