#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vectors.h>
#include <shaderprogram.h>
#include <vector>
#include <shadersources.h>
#include <triangle.h>
#include <cylinder.h>
#include <camera.h>
#include <scene.h>
#include <exception>
#include <chesstex.h>
#include <memory>
#include <quad.h>
#include <sph.h>
#include <snake_piece.h>
#include <obj_geometry.h>
#include <engine.h>
#include <cameracontrol.h>


// Define global objects like lights and cameras
PerspectiveCamera perspCam = PerspectiveCamera(vec3(3, -3, 2), vec3(0, 0, 0), 1, M_PI / 3);
OrthographicCamera orthoCam = OrthographicCamera(vec3(3, -3, 2), vec3(0, 0, 0), 1, 10, 100);
CameraController perspectiveController = CameraController(&perspCam);
CameraController orthographicController = CameraController(&orthoCam);

Light light = { vec4(-1, -3, 3, 0), vec4(1,1,1,1) };
Scene scene = Scene(&orthoCam, light, vec4(0.1, 0.1, 0.1, 0.1));

auto chessTex = std::make_unique<ChessTex>(vec4(184.0f/255, 135.0f/255, 98.0f/255, 1), vec4(233.0f/255, 211.0f/255, 173.0f/255, 1),25,25);
auto snakeTex = std::make_unique<ChessTex>(vec4(0.1f,1,0,1), vec4(0,1,0.5f,1),1,8);

//create materials that use these textures
Material chessMaterial = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, chessTex.get()};
Material snakeMaterial = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, snakeTex.get()};

//load or generate geometries
auto snakeTailGeometry = std::make_unique<SnakePiece>(0.2,0,vec2(-1,0), vec2(0,1));
auto sphereGeometry = std::make_unique<Sphere>(50);
auto monkeyGeometry = std::make_unique<Obj_geometry>("monkey3.obj");


// the init function sets up the scene
// later it will be registered to be called after opengl loaded
void init() {
	scene.light = light;

	auto snakeTail = std::make_unique<Object>(std::move(snakeTailGeometry), snakeMaterial);
	auto sphereObject = std::make_unique<Object>(std::move(sphereGeometry), chessMaterial);
	auto monkeyObject = std::make_unique<Object>(std::move(monkeyGeometry), chessMaterial);

	snakeTail->position = vec3(1, -1, 0.3);
	snakeTail->scale = vec3(1, 1, 1);

	sphereObject->position = vec3(-1, -1, 1);
	sphereObject->scale = vec3(0.5f, 0.5f, 0.5f);

	monkeyObject->position = vec3(0.0f, 0.0f, 1.0f);
	monkeyObject->scale = vec3(0.5f, 0.5f, 0.5f);

	scene.addObject(std::move(monkeyObject));
	scene.addObject(std::move(snakeTail));
	scene.addObject(std::move(sphereObject));

	auto ground = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, -1, 0), vec3(1, 1, 0)), chessMaterial);
	float sf = sqrtf(5.0f);
	ground->scale = vec3(sf, sf, sf);
	scene.addObject(std::move(ground));
	scene.addTexture(std::move(chessTex));
	scene.addTexture(std::move(snakeTex));

	scene.create();
}

void onUpdate(long deltaTime) {
	if (scene.getAllObjects().size() > 0) {
		Object* firstObj = (scene.getAllObjects())[0];
		firstObj->rotAngle += deltaTime / 1000.0f;
		firstObj->rotAxis = vec3(0, 0, 1);
	}
}

void onKeyPressed(char c) {
	if (c == 'c') {
		if (scene.camera == &perspCam) {
			scene.camera = &orthoCam;
		}
		else {
			scene.camera = &perspCam;
		}
	}
}


int main(int argc, char* argv[]) {
	// call init after opengl has been initialized
	Engine::initPostGL.addObserver(std::make_shared<Observer<void>>(init));

	// register update function
	Engine::update.addObserver(std::make_shared<Observer<long>>(onUpdate));

	// register keypress function
	Engine::onKeyDown.addObserver(std::make_shared<Observer<char>>(onKeyPressed));


	// start the program
	Engine::initialize(&argc, argv);
	Engine::activeScene = &scene;
	Engine::start();
	return 1;
}
