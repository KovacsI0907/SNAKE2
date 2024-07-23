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
#include <snakepiece.h>


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

auto chessTex = std::make_unique<ChessTex>(vec4(184.0f/255, 135.0f/255, 98.0f/255, 1), vec4(233.0f/255, 211.0f/255, 173.0f/255, 1), 9,9);
Material chessMat = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, chessTex.get()};

float groundSize = 1.0f;
float sceneRadius = 2 * groundSize;

vec2 mousePos = vec2(0, 0);
vec2 lastMousePos = vec2(0, 0);
vec2 mousePosDelta = vec2(0, 0);

long time = 0;
long lastTime = 0;
const long TICK_LENGTH = 500;
long tickTime = 0;

bool keys[256] = { false };

int snakePiecesWaiting = 5;
vec2 direction = vec2(1, 0);
vec2 headPos = vec2(0, 0);
std::vector<SnakePiece*> pieces;

void initialize() {

	auto spu = std::make_unique<SnakePiece>();
	pieces.push_back(spu.get());
	scene.addObject(std::move(spu));

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	scene.light = light;

	auto ground = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, -1, 0), vec3(1, 1, 0)), chessMat);
	ground->scale = vec3(groundSize, groundSize, groundSize);
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
	keys[key] = true;
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

void onKeyReleased(unsigned char key, int x, int y) {
	keys[key] = false;
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


void onTick() {
	headPos = headPos + direction;
	if (snakePiecesWaiting > 0) {
		snakePiecesWaiting--;
		auto newPiece = std::make_unique<SnakePiece>();
		SnakePiece* last = pieces.at(pieces.size() - 1);
		newPiece->moveTo(last->x, last->y);
		pieces.push_back(newPiece.get());
		scene.addObject(std::move(newPiece));
	}
	for (int i = pieces.size()-1; i > 0; i--) {
		SnakePiece* next = pieces.at(i - 1);
		pieces.at(i)->moveTo(next->x, next->y);
	}
	pieces.at(0)->moveTo(headPos.x, headPos.y);
}


void onUpdate() {
	lastTime = time;
	time = glutGet(GLUT_ELAPSED_TIME);
	long ellapsedMillis = time - lastTime;

	if (keys['w']) {
		direction = vec2(0,1);
	}

	if (keys['s']) {
		direction = vec2(0,-1);
	}

	if (keys['a']) {
		direction = vec2(-1, 0);
	}

	if (keys['d']) {
		direction = vec2(1, 0);
	}

	tickTime += ellapsedMillis;
	if (tickTime > TICK_LENGTH) {
		onTick();
		tickTime = 0;
	}

	glutPostRedisplay();
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
	glutKeyboardUpFunc(onKeyReleased);
	glutMouseFunc(onMouse);
	glutMotionFunc(onPressedMouseMovement);
	glutMouseWheelFunc(onMouseWheel);
	glutIdleFunc(onUpdate);

	glutMainLoop();
	return 1;
}
