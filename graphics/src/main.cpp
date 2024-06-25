#define GLEW_STATIC
#define FREEGLUT_STATIC
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vectors.h>
#include <shaderprogram.h>
#include <vector>
#include <shadersources.h>

using namespace std;

const unsigned int windowWidth = 640, windowHeight = 480;

unsigned int vao;
ShaderProgram shaderProgram = ShaderProgram(VERTEX_SOURCE, FRAGMENT_SOURCE);

void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vector<vec2> vertices;
	vertices.push_back(vec2(- 1, -1));
	vertices.push_back(vec2(0, 0));
	vertices.push_back(vec2(0, -0.5));
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	shaderProgram.compile();
	shaderProgram.use();
}

void onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram.setUniform("color", vec3(1.0f, 0.0f, 1.0f));
	shaderProgram.setUniform("MVP", mat4::identity());

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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