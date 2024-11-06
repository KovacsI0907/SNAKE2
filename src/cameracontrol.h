#pragma once
#include <camera.h>
#include <observer.h>
#include <engine.h>
#include <vectors.h>


class CameraController {
	const float zoomMultiplier = 1.1f;
	const float elevateMultiplier = 1.0f;
	const float anglePerPixelHorizontal = M_PI / Engine::getWidth();
	const float anglePerPixelVertical = M_PI / Engine::getWidth();
	const float horizontalDeadZoneAngle = M_PI / 8;
	Camera* cam;
	vec3 lookAt = vec3(0, 0, 0);
	vec3 eye = vec3(1, 1, 1);

public:
	CameraController(Camera* cam) {
		this->cam = cam;
		Engine::onMouseDragged.addObserver(std::make_shared<Observer<vec2>>([this](vec2 v) { rotate(v); }));
		Engine::onMouseScroll.addObserver(std::make_shared<Observer<int>>([this](int i) { zoom(i); }));
		Engine::update.addObserver(std::make_shared<Observer<long>>([this](long l) { onUpdate(l); }));
	}

private:
	void onUpdate(long deltaTime) {
		vec2 panDir(0, 0);
		if (Engine::isKeyPressed('a')) {
			panDir = panDir + vec2(-1.0f, 0);
		}
		if (Engine::isKeyPressed('d')) {
			panDir = panDir + vec2(1, 0);
		}
		if (Engine::isKeyPressed('w')) {
			panDir = panDir + vec2(0, 1);
		}
		if (Engine::isKeyPressed('s')) {
			panDir = panDir + vec2(0, -1.0f);
		}
		if (Engine::isKeyPressed('r')) {
			elevate(deltaTime, true);
		}
		if (Engine::isKeyPressed('f')) {
			elevate(deltaTime, false);
		}
		pan(panDir, deltaTime);

		cam->eye = eye;
		cam->lookAt = lookAt;
	}

	vec3 getForward() {
		vec3 forward = normalize(lookAt - eye);
		forward.z = 0;
		return forward;
	}

	vec3 getRight() {
		return normalize(cross(getForward(), vec3(0, 0, 1)));
	}

	void pan(vec2 dir, long deltaTime) {

		vec3 pan = (getRight() * dir.x + getForward() * dir.y) * distanceFromOrig() * deltaTime/1000.0f;
		lookAt = lookAt + pan;
		eye = eye + pan;
	}

	void rotate(vec2 pixelDelta) {
		mat4 rotHorizontal = RotationMatrix(vec3(0, 0, 1), pixelDelta.x * anglePerPixelHorizontal);
		mat4 rotVertical = RotationMatrix(getRight(), pixelDelta.y * anglePerPixelVertical);
		vec4 temp = vec4(eye - lookAt, 0);
		temp = temp * rotHorizontal * rotVertical;

		vec3 newEye = lookAt + temp.xyz();
		float angleWithUp = acosf(dot(vec3(0, 0, 1), newEye - lookAt) / (newEye - lookAt).length());
		if (angleWithUp < horizontalDeadZoneAngle || M_PI - angleWithUp < horizontalDeadZoneAngle) {
			return;
		}
		eye = newEye;
	}

	void zoom(int direction) {
		
		vec3 newPos = eye - lookAt;
		if (direction < 0) {
			newPos = newPos * zoomMultiplier;
		}
		else {
			newPos = newPos / zoomMultiplier;
		}

		eye = newPos + lookAt;
	}

	void elevate(long deltaTime, bool up) {
		float zDelta = elevateMultiplier * distanceFromOrig() * (up?1.0f:-1.0f) * deltaTime / 1000.0f;
		lookAt.z += zDelta;
		eye.z += zDelta;
	}

	float distanceFromOrig() {
		return (eye - lookAt).length();
	}
};