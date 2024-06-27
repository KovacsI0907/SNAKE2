#include <camera.h>
#include <vectors.h>

mat4 PerspectiveCamera::view() {
	vec3 w = normalize(eye - lookAt);
	vec3 u = normalize(cross(preferredUp, w));
	vec3 v = cross(w,u);

	mat4 toOrigin = TranslateMatrix(eye * (-1.0f));
	mat4 viewTr = mat4(	u.x, v.x, w.x, 0,
					u.y, v.y, w.y, 0,
					u.z, v.z, w.z, 0,
					0,   0,   0,   1);
	
	return toOrigin * viewTr;
}

mat4 PerspectiveCamera::projection() {
	float scaleX = 1 / tanf(fov / 2) / aspectRatio;
	float scaleY = 1 / tanf(fov / 2);
	mat4 fovNormalize = mat4(scaleX, 0, 0, 0,
		0, scaleY, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	float a = (frontPlane + backPlane) / (frontPlane - backPlane);
	float b = (frontPlane * 2 * backPlane) / (frontPlane - backPlane);
	mat4 perspectiveTr = mat4(	1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, a,-1,
								0, 0, b, 0);

	return fovNormalize * perspectiveTr;
}

mat4 OrthographicCamera::view() {
	vec3 w = normalize(eye - lookAt);
	vec3 u = normalize(cross(preferredUp, w));
	vec3 v = cross(w, u);

	mat4 toOrigin = TranslateMatrix(eye * (-1.0f));
	mat4 viewTr = mat4(u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0,
		0, 0, 0, 1);

	return toOrigin * viewTr;
}

mat4 OrthographicCamera::projection() {
	return ScaleMatrix(vec3(2 / (aspectRatio * height), 2 / height, -2 / depth)) * TranslateMatrix(vec3(0, 0, -0.5));
}