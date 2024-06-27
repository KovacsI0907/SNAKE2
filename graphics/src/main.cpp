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

using namespace std;

const unsigned int windowWidth = 640, windowHeight = 480;

ShaderProgram shaderProgram = ShaderProgram(VERTEX_SOURCE, FRAGMENT_SOURCE, GEOMETRY_SOURCE);
Triangle tri = Triangle(vec3(0, 0, 0), vec3(1, 1, 0), vec3(1, 0, 0));

void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);
	
	shaderProgram.compile();
	shaderProgram.use();
}

void onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram.setUniform("color", vec3(1.0f, 0.0f, 1.0f));
	shaderProgram.setUniform("MVP", mat4::identity());

	glutSwapBuffers();
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

	glutMainLoop();
	return 1;
}
