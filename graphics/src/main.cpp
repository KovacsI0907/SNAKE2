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


const unsigned int windowWidth = 640, windowHeight = 480;

ShaderProgram shaderProgramGouraud = ShaderProgram(GOURAUD_VERTEX, GOURAUD_FRAGMENT);
ShaderProgram shaderProgramPhong = ShaderProgram(PHONG_VERTEX, PHONG_FRAGMENT);

PerspectiveCamera pCam = PerspectiveCamera(vec3(1, -2, 2), vec3(0, 0, 0), (float)windowWidth / windowHeight, M_PI / 3);
OrthographicCamera oCam = OrthographicCamera(vec3(1, -2, 2), vec3(0, 0, 0), (float)windowWidth / windowHeight, 3, 100);

Scene scene = Scene();
Light light = { vec4(3, -3, 3, 0), vec4(1,1,1,1) };

auto chessTex = std::make_unique<ChessTex>(vec4(1, 0, 0, 1), vec4(0, 0, 1, 1));
Material cylMat = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, chessTex.get()};
Material triMat = { vec4(1, 1, 1, 1), vec4(1, 1, 0, 0), vec4(0,0,0,1), 25.0f };
auto cyl = std::make_unique<Cylinder>(10);
auto tri = std::make_unique<Triangle>(vec3(-1.5, 0, 0), vec3(1.5, 0, 0), vec3(0, 0, 1));


void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);

	scene.shaderProgram = &shaderProgramGouraud;
	scene.camera = &pCam;
	scene.light = light;

	scene.addObject(std::make_unique<Object>(std::move(cyl), cylMat));
	scene.addObject(std::make_unique<Object>(std::move(tri), triMat));
	scene.addTexture(std::move(chessTex));
	
	try {
		shaderProgramGouraud.compile();
		shaderProgramGouraud.use();
		shaderProgramPhong.compile();
	}
	catch (const std::exception& e) {
		printf("%s\n", e.what());
	}
}

void onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.drawAll();

	glutSwapBuffers();
}

void onKeyPressed(unsigned char key, int x, int y) {
	if (key == 'c') {
		if (scene.camera == &pCam) {
			scene.camera = &oCam;
		}
		else {
			scene.camera = &pCam;
		}
	}

	if (key == 's') {
		if (scene.shaderProgram == &shaderProgramGouraud) {
			scene.shaderProgram = &shaderProgramPhong;
		}
		else {
			scene.shaderProgram = &shaderProgramGouraud;
		}
		scene.shaderProgram->use();
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

	glutMainLoop();
	return 1;
}
