#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <exception>
#include <string>
#include <vectors.h>

class ShaderException : public std::exception {
	std::string message;

public:

	ShaderException(const std::string& msg) : message(msg) {}

	ShaderException(const char* msg) : message(msg) {}

	virtual const char* what() const throw()
	{
		return message.c_str();
	}
};

class ShaderProgram {
	const char* vertexSource;
	const char* fragmentSource;
	const char* geometrySource;

	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;
	unsigned int geometryShader = 0;

	unsigned int programHandle = 0;
	bool linked = false;

public:
	ShaderProgram(const char* vertSrc, const char* fragSrc, const char* geoSrc = "") {
		vertexSource = vertSrc;
		fragmentSource = fragSrc;
		geometrySource = geoSrc;
	}

	void compile() {
		linked = false;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		if (vertexShader == 0)
			throw ShaderException("Error in vertex shader creation");

		//				shader handler,	num of strings,	array of c-strings,	length (NULL means null terminated)
		glShaderSource(	vertexShader,	1,				&vertexSource,		NULL);

		glCompileShader(vertexShader);

		int compileStatus;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);

		if (!compileStatus) {
			char logBuffer[2048];
			int logLength;
			glGetShaderInfoLog(vertexShader, 2048, &logLength, logBuffer);
			
			throw ShaderException(logBuffer);
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		if (vertexShader == 0)
			throw ShaderException("Error in fragment shader creation");

		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus) {
			char logBuffer[2048];
			int logLength;
			glGetShaderInfoLog(fragmentShader, 2048, &logLength, logBuffer);

			throw ShaderException(logBuffer);
		}

		programHandle = glCreateProgram();
		if (!programHandle) {
			throw ShaderException("Error in shader program creation");
		}

		glAttachShader(programHandle, vertexShader);
		glAttachShader(programHandle, fragmentShader);
		//TODO geometry shader

		glLinkProgram(programHandle);

		int linkStatus;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);

		if (!linkStatus) {
			char logBuffer[2048];
			int logLength;
			glGetProgramInfoLog(programHandle, 2048, &logLength, logBuffer);

			throw ShaderException(logBuffer);
		}
		linked = true;
	}

	void use() {
		if (!linked)
			throw ShaderException("Can't use shader, because it was not (successfully) compiled");

		glUseProgram(programHandle);
	}

	unsigned int getHandle() {
		return programHandle;
	}

	int getUnifromLocation(const char* name) {
		int location = glGetUniformLocation(programHandle, name);
		if (location == -1)
			throw ShaderException("Invalid uniform name");

		return location;
	}

	void setUniform(const char* name, unsigned int u) {
		int location = getUnifromLocation(name);
		glUniform1ui(location, u);
	}

	void setUniform(const char* name, int i) {
		int location = getUnifromLocation(name);
		glUniform1i(location, i);
	}

	void setUniform(const char* name, float f) {
		int location = getUnifromLocation(name);
		glUniform1f(location, f);
	}

	void setUniform(const char* name, vec2 v) {
		int location = getUnifromLocation(name);
		glUniform2fv(location, 1, &v.x);
	}

	void setUniform(const char* name, vec3 v) {
		int location = getUnifromLocation(name);
		glUniform3fv(location, 1, &v.x);
	}

	void setUniform(const char* name, vec4 v) {
		int location = getUnifromLocation(name);
		glUniform4fv(location, 1, &v.x);
	}

	void setUniform(const char* name, mat4 mat) {
		int location = getUnifromLocation(name);
		glUniformMatrix4fv(location, 1, GL_TRUE, &mat[0][0]);
	}

	~ShaderProgram() {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);

		glDeleteProgram(programHandle);
	}
};