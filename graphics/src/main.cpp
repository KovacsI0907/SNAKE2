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


const unsigned int windowWidth = 640, windowHeight = 480;

ShaderProgram shaderProgram = ShaderProgram(VERTEX_SOURCE, FRAGMENT_SOURCE);
PerspectiveCamera pCam = PerspectiveCamera(vec3(1, -2, 2), vec3(0, 0, 0), (float)windowWidth / windowHeight, M_PI / 3);
OrthographicCamera oCam = OrthographicCamera(vec3(1, -2, 2), vec3(0, 0, 0), (float)windowWidth / windowHeight, 3, 100);
Camera* activeCam = &pCam;
Cylinder cyl = Cylinder(50);
Triangle tri = Triangle(vec3(-1.5, 0, 0), vec3(1.5, 0, 0), vec3(0, 0, 1));

void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);

	cyl.create();
	tri.create();
	
	shaderProgram.compile();
	shaderProgram.use();
}

void onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.setUniform("color", vec3(1.0f, 0.0f, 1.0f));
	shaderProgram.setUniform("MVP", activeCam->view() * activeCam->projection());

	tri.draw();
	shaderProgram.setUniform("color", vec3(1.0f, 1.0f, 0.0f));
	cyl.draw();

	glutSwapBuffers();
}

void onKeyPressed(unsigned char key, int x, int y) {
	if (key == 'c') {
		if (activeCam == &pCam) {
			activeCam = &oCam;
		}
		else {
			activeCam = &pCam;
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
