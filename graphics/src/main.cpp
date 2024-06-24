#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vectors.h>
#include <shaderprogram.h>
#include <vector>

using namespace std;

const unsigned int windowWidth = 640, windowHeight = 480;

const char* vertexSource = R"(
	#version 330
	precision highp float;

	uniform mat4 MVP;
	layout(location = 0) in vec2 vp;

	void main() {
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;
	}
)";

const char* fragmentSource = R"(
	#version 330
	precision highp float;
	
	uniform vec3 color;
	out vec4 outColor;

	void main() {
		outColor = vec4(color,1);
	}
)";

ShaderProgram shaderProgram = ShaderProgram(vertexSource, fragmentSource);

class Object {
protected:
	unsigned int vao;
	unsigned int vbo;
	vector<vec2> vertices;

	void updateGpu() {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	}

public:
	void create() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}
	void addVertex(vec2 v) {
		vertices.push_back(v);
		updateGpu();
	}

	virtual void draw() = 0;
};

class MyShape : public Object {
public:
	virtual void draw() {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		if (vertices.size() >= 3) {
			shaderProgram.setUniform("color", vec3(1, 1, 0));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

			shaderProgram.setUniform("color", vec3(0, 0, 0));
			for (int i = 2; i < vertices.size(); i++) {
				glDrawArrays(GL_LINE_LOOP, i - 2, 3);
			}
		}

		shaderProgram.setUniform("color", vec3(1, 0, 0));
		glDrawArrays(GL_POINTS, 0, vertices.size());
	}
};





MyShape shape;
float scale = 1.0f;
vec2 cameraPos = vec2(0, 0);
bool keys[256] = { 0 };
long time = 0;

void initialize() {
	glViewport(0, 0, windowWidth, windowHeight);
	glPointSize(5);
	glLineWidth(3);
	shape.create();
	
	shaderProgram.compile();
	shaderProgram.use();
}

void onDisplay() {
	glClearColor(0.35, 0.35, 0.35, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	shaderProgram.setUniform("color", vec3(1.0f, 0.0f, 1.0f));
	shaderProgram.setUniform("MVP", TranslateMatrix(vec3(cameraPos * -1, 0)) * ScaleMatrix(vec3(scale, scale, scale)));

	shape.draw();

	glutSwapBuffers();
}

vec2 pixelToNDC(vec2 clickPos) {
	float x = (clickPos.x / windowWidth) * 2 - 1;
	float y = (windowHeight - clickPos.y) / windowHeight * 2 - 1;

	return vec2(x, y);
}

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		shape.addVertex(pixelToNDC(vec2(x, y)) * (1 / scale) + cameraPos);
	}
}

void onMouseWheel(int wheel, int direction, int x, int y) {
	scale *= direction > 0 ? 1.1f : 1 / 1.1f;
	glutPostRedisplay();
}


void onKeyDown(unsigned char key, int x, int y) {
	keys[key] = true;
}

void onKeyUp(unsigned char key, int x, int y) {
	keys[key] = false;
}

void update(long deltaTimeMillis) {
	float deltaTimeSec = deltaTimeMillis / 1000.0f;
	float cameraSpeedX = 1.0f; // units/sec == half screen width / sec
	float cameraSpeedY = cameraSpeedX * windowHeight / windowWidth;
	if (keys['w']) {
		cameraPos = cameraPos + vec2(0, 1) * cameraSpeedY * deltaTimeSec;
	}
	if (keys['s']) {
		cameraPos = cameraPos + vec2(0, -1) * cameraSpeedY * deltaTimeSec;
	}
	if (keys['d']) {
		cameraPos = cameraPos + vec2(1, 0) * cameraSpeedX * deltaTimeSec;
	}
	if (keys['a']) {
		cameraPos = cameraPos + vec2(-1, 0) * cameraSpeedX * deltaTimeSec;
	}
}

void onIdle() {
	long timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	long deltaTimeMillis = timeSinceStart - time;
	time = timeSinceStart;
	update(deltaTimeMillis);
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
	glutMouseFunc(onMouse);
	glutMouseWheelFunc(onMouseWheel);
	glutKeyboardUpFunc(onKeyUp);
	glutKeyboardFunc(onKeyDown);
	glutIdleFunc(onIdle);

	glutMainLoop();
	return 1;
}