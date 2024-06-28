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


const unsigned int windowWidth = 640, windowHeight = 480;

ShaderProgram shaderProgram = ShaderProgram(GOURAUD_VERTEX, GOURAUD_FRAGMENT);
PerspectiveCamera pCam = PerspectiveCamera(vec3(1, -2, 2), vec3(0, 0, 0), (float)windowWidth / windowHeight, M_PI / 3);
OrthographicCamera oCam = OrthographicCamera(vec3(1, -2, 2), vec3(0, 0, 0), (float)windowWidth / windowHeight, 3, 100);

Scene scene = Scene();
Light light = { vec4(3, -3, 3, 0), vec4(1,1,1,1) };

Material cylMat = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f };
Material triMat = { vec4(1, 1, 1, 1), vec4(1, 1, 0, 0), vec4(0,0,0,1), 25.0f };
Cylinder* cyl = new Cylinder(50);
Triangle* tri = new Triangle(vec3(-1.5, 0, 0), vec3(1.5, 0, 0), vec3(0, 0, 1));


void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);

	scene.shaderProgram = &shaderProgram;
	scene.camera = &pCam;
	scene.light = light;

	Object* c = new Object(cyl, cylMat);
	Object* t = new Object(tri, triMat);

	scene.addObject(c);
	scene.addObject(t);
	
	try {
		shaderProgram.compile();
		shaderProgram.use();
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
