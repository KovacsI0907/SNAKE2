#pragma once

#include <shaderprogram.h>
#include <shadersources.h>
#include <scene.h>
#include <camera.h>
#include <observer.h>

enum MouseKey {
	left, right, middle
};

class Engine {
protected:
	static unsigned int width, height;
	static ShaderProgram mainShader, depthShader;

	static unsigned int depthMapResolution;
	static unsigned int depthMapFBO;
	static unsigned int depthMapTexture;

	static vec2 mouseDragPos;
	
	static void onDisplay();
	static void onIdle();
	static void onMouse(int button, int state, int x, int y);
	static void onKeyUpFunc(unsigned char key, int x, int y);
	static void onKeyDownFunc(unsigned char key, int x, int y);
	static void onMouseWheel(int button, int dir, int x, int y);
	static void onMouseMovementFunc(int x, int y);
	static void onMousePressedMovement(int x, int y);
	static OrthographicCamera getDepthCam(float shadowRadius, vec4 lightPos);
	static void loadUniforms(Object* object, Camera* camera, ShaderProgram* shaderProgram);

	static long time;
	static bool keys[256];

public:
	static Scene* activeScene;
	

	Engine() = delete;
	Engine(Engine& other) = delete;
	void operator=(const Engine&) = delete;

	static void initialize(int* argc, char* argv[]);
	static void start();
	static void render(Scene* scene);
	static bool isKeyPressed(char key);

	static unsigned int getWidth();
	static unsigned int getHeight();

	static Event<void> initPreGL;
	static Event<void> initPostGL;

	static Event<void> preDraw;
	static Event<long> update;

	static Event<char> onKeyUp;
	static Event<char> onKeyDown;

	static Event<MouseKey> onMouseUp;
	static Event<MouseKey> onMouseDown;
	static Event<int> onMouseScroll;

	static Event<vec2> onMouseMovement;
	static Event<vec2> onMouseDragged;

};