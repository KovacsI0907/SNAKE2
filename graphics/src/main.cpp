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

	float vertices[] = { -0.8,-0.8,-0.6,1.0,0.8,-0.2 };
	unsigned int vbo;
void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);
	glLineWidth(5.0f);
	glPointSize(7.0f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	shaderProgram.compile();
	shaderProgram.use();
}

float yTransform = 0, xTransform = 0, scale = 1;
std::vector<float> vertexData;

void onDisplay() {
		glClearColor(0.3f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	if (vertexData.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_DYNAMIC_DRAW);

		shaderProgram.setUniform("color", vec3(0.2, 0.9, 0.7));
		shaderProgram.setUniform("MVP", mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-yTransform, -xTransform, 0, 1) * mat4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1));
		glBindVertexArray(vao);
		shaderProgram.setUniform("color", vec3(1, 1, 0));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexData.size() / 2);
		shaderProgram.setUniform("color", vec3(0, 0, 0));
		for (int i = 2; i < (vertexData.size() / 2); i++)
		{
			glDrawArrays(GL_LINE_LOOP, i-2, 3);
		}
		shaderProgram.setUniform("color", vec3(1, 0, 0));
		glDrawArrays(GL_POINTS, 0, vertexData.size() / 2);
	}

	glutSwapBuffers();
}



void onKeyPressed(unsigned char key, int x, int y)
{
	if(key == 'w')
		xTransform += 0.05;
	if (key == 's')
		xTransform -= 0.05;
	if (key == 'a')
		yTransform -= 0.05;
	if (key == 'd')
		yTransform += 0.05;
	glutPostRedisplay();
}

void mouseWheelFunc(int wheel, int direction, int x, int y)
{
	if (direction > 0)
	{
		scale *=1.2f;
		yTransform += y / windowHeight /10/scale;
		xTransform += x / windowWidth /10/scale;
	}
	if (direction < 0)			 
	{
		scale/=1.2f;
		yTransform += y / windowHeight / 10 / scale;
		xTransform += x / windowWidth / 10 / scale;
	}
	
	glutPostRedisplay();
}


void mouseFunc(int button, int state, int x, int y)
{
	if (button == 0 && state == 0)
	{
		float xc = (2.0f * x) / windowWidth - 1.0f;
		float yc = 1.0f - (2.0f * y) / windowHeight;

		vec4 xy = vec4(xc, yc,0,1);

		xy = xy * mat4(1 / scale, 0, 0, 0, 0, 1 / scale, 0, 0, 0, 0, 1 / scale, 0, 0, 0, 0, 1) * mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, yTransform, xTransform, 0, 1);

		vertexData.push_back(xy.x);
		vertexData.push_back(xy.y);
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
	glutMouseWheelFunc(mouseWheelFunc);
	glutMouseFunc(mouseFunc);

	glutMainLoop();
	return 1;
}

