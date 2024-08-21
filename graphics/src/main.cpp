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

PerspectiveCamera pCam = PerspectiveCamera(vec3(1, -3, 3), vec3(0, 0, 0), 1, M_PI / 3);
OrthographicCamera oCam = OrthographicCamera(vec3(1, -5, 5), vec3(0, 0, 0), 1, 10, 100);

Light light = { vec4(-1, -3, 3, 0), vec4(1,1,1,1) };
Scene scene = Scene(&pCam, light, vec4(0.1, 0.1, 0.1, 0.1));

auto chessTex = std::make_unique<ChessTex>(vec4(184.0f/255, 135.0f/255, 98.0f/255, 1), vec4(233.0f/255, 211.0f/255, 173.0f/255, 1),25,25);
auto snakeTex = std::make_unique<ChessTex>(vec4(0.1f,1,0,1), vec4(0,1,0.5f,1),1,8);
Material cylMat = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, chessTex.get()};
Material snkMat = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, snakeTex.get()};
auto snk = std::make_unique<SnakePiece>(0.2,0,vec2(-1,0), vec2(0,1));
auto sph = std::make_unique<Sphere>(50);
//auto obj = std::make_unique<Obj_geometry>("torus.obj");


void init() {
	scene.light = light;


	auto quad = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, 0, 0), vec3(-1, 0, 2), vec3(1, 0, 0), vec3(1, 0, 2)), cylMat);
	quad->scale = vec3(0.5, 0.5, 0.5);
	quad->position = vec3(2, 0, 0);
	auto quad2 = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, 0, 0), vec3(-1, 0, 2), vec3(1, 0, 0), vec3(1, 0, 2)), cylMat);
	quad2->scale = vec3(0.5, 0.5, 0.5);
	quad2->position = vec3(0, 3, 0);

	auto snkObj = std::make_unique<Object>(std::move(snk), snkMat);
	auto sphObj = std::make_unique<Object>(std::move(sph), cylMat);
	//auto objObj = std::make_unique<Object>(std::move(obj), cylMat);

	snkObj->position = vec3(0, 0, 0.3);
	snkObj->scale = vec3(1, 1, 1);

	sphObj->position = vec3(0, 0, 1);
	sphObj->scale = vec3(1, 1, 1);

	scene.addObject(std::move(snkObj));
	//scene.addObject(std::move(objObj));

	//scene.addObject(std::move(sphObj));

	scene.addObject(std::move(quad));
	scene.addObject(std::move(quad2));
	float sf = sqrtf(5.0f);
	auto ground = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, -1, 0), vec3(1, 1, 0)), cylMat);
	ground->scale = vec3(sf, sf, sf);
	scene.addObject(std::move(ground));
	scene.addTexture(std::move(chessTex));

	scene.create();
}


int main(int argc, char* argv[]) {
	Engine::initPostGL.addObserver(std::make_shared<Observer<void>>(init));
	Engine::onKeyUp.addObserver(std::make_shared<Observer<char>>([](char c) {printf("%c released\n", c); }));

	Engine::initialize(&argc, argv);
	Engine::activeScene = &scene;
	Engine::start();
	return 1;
}
