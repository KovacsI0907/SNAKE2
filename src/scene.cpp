#include <scene.h>
#include <texture.h>

void Scene::loadUniforms(Object* object, Camera* camera, ShaderProgram* shaderProgram) {
	mat4 M = object->getModel();
	mat4 MVP = M * camera->view() * camera->projection();
	mat4 Minv = object->getModelInverse();

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

OrthographicCamera Scene::getDepthCam(float maxSceneRadius, vec4 lightPos) {
	vec3 dir = normalize(lightPos.xyz());
	vec3 eye = dir * maxSceneRadius;
	vec3 lookAt = vec3(0, 0, 0);
	float asp = 1.0f;
	float height = maxSceneRadius * 2;
	float depth = maxSceneRadius * 2;
	vec3 preferredUp = vec3(0, 0, 1);

	return OrthographicCamera(eye, lookAt, asp, height, depth, preferredUp);
}

void Scene::render(Camera* normalCamera, float maxSceneRadius, ShaderProgram* normalShader, ShaderProgram* shadowShader, int windowWidth, int windowHeight) {
	//depth map

	//create depth camera
	OrthographicCamera depthCam = getDepthCam(maxSceneRadius, light.position);

	//select depth framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, depthMapResolution, depthMapResolution);
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);

	//render depth map

	//select shader
	shadowShader->use();
	for (auto& object : objects) {
		shadowShader->setUniform("lightMVP", object->getModel() * depthCam.view() * depthCam.projection());
		object->draw();
	}

	//normal render

	//select normal framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render normal scene

	//select normal shader
	normalShader->use();

	//bind shadow texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	normalShader->setUniform("depthMap", 1);

	for (auto& object : objects) {
		normalShader->setUniform("lightMVP", object->getModel() * depthCam.view() * depthCam.projection());
		loadUniforms(object.get(), normalCamera, normalShader);
		object->draw();
	}
}

void Scene::renderScene(Camera* camera, ShaderProgram* shaderProgram, int winHeight, int winWidth) {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, winWidth, winHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	shaderProgram->setUniform("depthMap", GL_TEXTURE1);

	shaderProgram->use();

	for (auto& object : objects) {
		loadUniforms(object.get(), camera, shaderProgram);
		object->draw();
	}
}

void Scene::create() {
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMapTexture);

	// create empty texture
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapResolution, depthMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// attach framebuffer to texture
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// reset framebuffer for rendering visible scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const std::vector<Object*> Scene::getShadowCasters() {
	std::vector<Object*> shadowCasters;
	for (auto& obj : objects) {
		for (Object* child : obj->getAllChildrenRecurse()) {
			if (child->castsShadow) {
				shadowCasters.push_back(child);
			}
		}
	}

	return shadowCasters;
}

const std::vector<Object*> Scene::getAllObjects() {
	std::vector<Object*> all;
	for (auto& obj : objects) {
		for (Object* child : obj->getAllChildrenRecurse()) {
			all.push_back(child);
		}
	}
	return all;
}