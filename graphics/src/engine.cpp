#include <engine.h>
#include <GL/glut.h>

unsigned int Engine::width = 640;
unsigned int Engine::height = 480;
ShaderProgram Engine::mainShader = ShaderProgram(PHONG_VERTEX, PHONG_FRAGMENT);
ShaderProgram Engine::depthShader = ShaderProgram(DEPTH_VERTEX, DEPTH_FRAGMENT);
Scene* Engine::activeScene = nullptr;
unsigned int Engine::depthMapResolution = 1024;
unsigned int Engine::depthMapFBO = -1;
unsigned int Engine::depthMapTexture = -1;

Event<void> Engine::initPreGL;
Event<void> Engine::initPostGL;

Event<void> Engine::preDraw;
Event<long> Engine::update;

Event<char> Engine::onKeyUp;
Event<char> Engine::onKeyDown;

Event<MouseKey> Engine::onMouseUp;
Event<MouseKey> Engine::onMouseDown;
Event<int> Engine::onMouseScroll;

long Engine::time = 0;
bool Engine::keys[256] = { false };

void Engine::initialize(int* argc, char* argv[]) {
	// Event before opengl is initialized
	initPreGL.notifyObservers();

	//GLUT
	glutInit(argc, argv);
	glutInitContextVersion(3, 3);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glewExperimental = true;
	glewInit();

	//GL
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	try {
		mainShader.compile();
		depthShader.compile();
		mainShader.use();
	}
	catch (const std::exception& e) {
		printf("SHADER ERROR:\n%s\n", e.what());
	}

	glutDisplayFunc(onDisplay);
	glutIdleFunc(onIdle);
	glutMouseFunc(onMouse);
	glutKeyboardUpFunc(onKeyUpFunc);
	glutKeyboardFunc(onKeyDownFunc);
	glutMouseWheelFunc(onMouseWheel);

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

	// Event after initializing engine
	initPostGL.notifyObservers();
}

void Engine::onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Event before rendering
	preDraw.notifyObservers();

	if (activeScene != nullptr) {
		render(activeScene);
	}

	glutSwapBuffers();
}

OrthographicCamera Engine::getDepthCam(float maxSceneRadius, vec4 lightPos) {
	vec3 dir = normalize(lightPos.xyz());
	vec3 eye = dir * maxSceneRadius;
	vec3 lookAt = vec3(0, 0, 0);
	float asp = 1.0f;
	float height = maxSceneRadius * 2;
	float depth = maxSceneRadius * 2;
	vec3 preferredUp = vec3(0, 0, 1);

	return OrthographicCamera(eye, lookAt, asp, height, depth, preferredUp);
}


void Engine::render(Scene* scene) {
	//depth map

	//create depth camera
	OrthographicCamera depthCam = getDepthCam(scene->radius, scene->light.position);

	//select depth framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, depthMapResolution, depthMapResolution);
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);

	//render depth map

	//select shader
	depthShader.use();
	for (Object* object : scene->getShadowCasters()) {
		depthShader.setUniform("lightMVP", object->getModel() * depthCam.view() * depthCam.projection());
		object->draw();
	}

	//normal render

	//select normal framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render normal scene

	//select normal shader
	mainShader.use();

	//bind shadow texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	mainShader.setUniform("depthMap", 1);

	for (Object* object : scene->getAllObjects()) {
		mainShader.setUniform("lightMVP", object->getModel() * depthCam.view() * depthCam.projection());
		loadUniforms(object, scene->camera, &mainShader);
		object->draw();
	}
}

void Engine::loadUniforms(Object* object, Camera* camera, ShaderProgram* shaderProgram) {
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


	shaderProgram->setUniform("radAmbient", activeScene->La);
	shaderProgram->setUniform("radLight", activeScene->light.intensity);

	shaderProgram->setUniform("coeffAmbient", object->material.kAmbient);
	shaderProgram->setUniform("coeffDiffuse", object->material.kDiffuse);
	shaderProgram->setUniform("coeffSpecular", object->material.kSpecular);

	shaderProgram->setUniform("shine", object->material.shininess);

	shaderProgram->setUniform("wEyePos", camera->eye);
	shaderProgram->setUniform("wLightPos", activeScene->light.position);

	shaderProgram->setUniform("MVP", MVP);
	shaderProgram->setUniform("M", M);
	shaderProgram->setUniform("Minv", Minv);
}

void Engine::start() {
	glutMainLoop();
}

void Engine::onIdle() {
	long currentTime = glutGet(GLUT_ELAPSED_TIME);
	long deltaTime = currentTime - time;
	time = currentTime;
	update.notifyObservers(deltaTime);
}

void Engine::onMouse(int button, int state, int x, int y) {
	MouseKey key;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		key = left;
		break;
	case GLUT_MIDDLE_BUTTON:
		key = middle;
		break;
	case GLUT_RIGHT_BUTTON:
		key = right;
		break;
	default:
		key = left;
	}

	if (state == GLUT_UP) {
		onMouseUp.notifyObservers(key);
	}
	else {
		onMouseDown.notifyObservers(key);
	}
}

void Engine::onKeyUpFunc(unsigned char key, int x, int y) {
	keys[key] = false;
	onKeyUp.notifyObservers(key);
}

void Engine::onKeyDownFunc(unsigned char key, int x, int y) {
	keys[key] = true;
	onKeyDown.notifyObservers(key);
}

void Engine::onMouseWheel(int button, int dir, int x, int y)
{
	onMouseScroll.notifyObservers(dir);
}