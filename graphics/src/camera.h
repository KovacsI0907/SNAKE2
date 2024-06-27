#pragma once

#include <vectors.h>

class Camera {
public:
	vec3 eye;
	vec3 lookAt;
	vec3 preferredUp;
	float aspectRatio;

	Camera(vec3 eye, vec3 lookAt, float aspectRatio, vec3 preferredUp = vec3(0,0,1)) {
		this->eye = eye;
		this->lookAt = lookAt;
		this->aspectRatio = aspectRatio;
		this->preferredUp = preferredUp;
	}

	virtual mat4 view() = 0;
	virtual mat4 projection() = 0;
};

class PerspectiveCamera : public Camera {
private:
	float frontPlane;
	float backPlane;
public:
	float fov; //vertical

	PerspectiveCamera(vec3 eye, vec3 lookAt, float asp, float verticalFov, vec3 preferredUp = vec3(0,0,1), float frontPlane = 0.1f, float backPlane = 100.0f) : Camera(eye, lookAt, asp, preferredUp) {
		this->frontPlane = frontPlane;
		this->backPlane = backPlane;
		fov = verticalFov;
	}
	mat4 view();
	mat4 projection();
};

class OrthographicCamera : public Camera {
public:
	float height;
	float depth;

	OrthographicCamera(vec3 eye, vec3 lookAt, float aspectRatio, float height, float depth, vec3 preferredUp = vec3(0, 0, 1)) : Camera(eye, lookAt, aspectRatio, preferredUp) {
		this->height = height;
		this->depth = depth;
	}

	mat4 view();
	mat4 projection();
};