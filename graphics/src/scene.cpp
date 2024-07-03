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

void Scene::addObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

void Scene::addTexture(std::unique_ptr<Texture> texture) {
	if (!texture->created())
		texture->create();
	textures.push_back(std::move(texture));
}

void Scene::deleteObject(Object* object) {
	for (auto it = objects.begin(); it != objects.end(); it++) {
		if (it->get() == object) {
			objects.erase(it);
			break;
		}
	}
}

void Scene::deleteTexture(Texture* texture) {
	for (auto it = textures.begin(); it != textures.end(); it++) {
		if (it->get() == texture) {
			textures.erase(it);
			break;
		}
	}
}

void Scene::drawAll() {
	for (auto& object : objects) {
		loadUniforms(object.get());
		object->draw();
	}
}