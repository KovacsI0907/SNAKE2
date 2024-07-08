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


const unsigned int windowWidth = 600, windowHeight = 600;

ShaderProgram shaderProgramGouraud = ShaderProgram(GOURAUD_VERTEX, GOURAUD_FRAGMENT);
ShaderProgram shaderProgramPhong = ShaderProgram(PHONG_VERTEX, PHONG_FRAGMENT);
ShaderProgram depthShader = ShaderProgram(DEPTH_VERTEX, DEPTH_FRAGMENT);
ShaderProgram* activeShaders = &shaderProgramGouraud;

PerspectiveCamera pCam = PerspectiveCamera(vec3(1, -3, 3), vec3(0, 0, 0), (float)windowWidth / windowHeight, M_PI / 3);
OrthographicCamera oCam = OrthographicCamera(vec3(1, -5, 5), vec3(0, 0, 0), (float)windowWidth / windowHeight, 10, 100);
Camera* activeCamera = &pCam;

Scene scene = Scene();
Light light = { vec4(-1, -3, 3, 0), vec4(1,1,1,1) };

auto chessTex = std::make_unique<ChessTex>(vec4(184.0f/255, 135.0f/255, 98.0f/255, 1), vec4(233.0f/255, 211.0f/255, 173.0f/255, 1), 25,25);
Material cylMat = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, chessTex.get()};
auto sph = std::make_unique<Sphere>(50);

float sceneRadius = 5.0f;

vec2 mousePos = vec2(0, 0);
vec2 lastMousePos = vec2(0, 0);
vec2 mousePosDelta = vec2(0, 0);


void initialize() {

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	scene.light = light;


	auto quad = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, 0, 0), vec3(-1, 0, 2), vec3(1, 0, 0), vec3(1, 0, 2)), cylMat);
	quad->scale = vec3(0.5, 0.5, 0.5);
	quad->position = vec3(0, 0, 0.75);

	auto sphObj = std::make_unique<Object>(std::move(sph), cylMat);
	sphObj->position = vec3(0, -1, 1);
	sphObj->scale = vec3(0.5, 0.5, 0.5);

	scene.addObject(std::move(sphObj));

	scene.addObject(std::move(quad));
	float sf = sqrtf(5.0f);
	auto ground = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, -1, 0), vec3(1, 1, 0)), cylMat);
	ground->scale = vec3(sf, sf, sf);
	scene.addObject(std::move(ground));
	scene.addTexture(std::move(chessTex));

	scene.create();
	
	try {
		shaderProgramGouraud.compile();
		shaderProgramPhong.compile();
		depthShader.compile();
		activeShaders->use();
	}
	catch (const std::exception& e) {
		printf("%s\n", e.what());
	}
}

void onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.render(activeCamera, sceneRadius, activeShaders, &depthShader, windowWidth, windowHeight);

	glutSwapBuffers();
}

void onKeyPressed(unsigned char key, int x, int y) {
	if (key == 'c') {
		if (activeCamera == &pCam) {
			activeCamera = &oCam;
		}
		else {
			activeCamera = &pCam;
		}
	}

	if (key == 's') {
		if (activeShaders == &shaderProgramGouraud) {
			activeShaders = &shaderProgramPhong;
		}
		else {
			activeShaders = &shaderProgramGouraud;
		}
		activeShaders->use();
	}
	glutPostRedisplay();
}

void onPressedMouseMovement(int x, int y) {
	mousePos = vec2(x, y);
	mousePosDelta = mousePos - lastMousePos;
	lastMousePos = mousePos;
	oCam.rotateAroundLookat(mousePosDelta / 100);
	pCam.rotateAroundLookat(mousePosDelta / 100);
	glutPostRedisplay();
}

void onMouseWheel(int wheel, int direction, int x, int y) {
	if (direction == 1) {
		oCam.moveCloserToLookat(1 / 1.1f);
		pCam.moveCloserToLookat(1 / 1.1f);
	}
	else {
		oCam.moveCloserToLookat(1.1f);
		pCam.moveCloserToLookat(1.1f);
	}

	glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mousePos = lastMousePos = vec2(x, y);
		}
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(3,3);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glewExperimental = true;
	glewInit();

	initialize();

	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKeyPressed);
	glutMouseFunc(onMouse);
	glutMotionFunc(onPressedMouseMovement);
	glutMouseWheelFunc(onMouseWheel);

	glutMainLoop();
	return 1;
}
