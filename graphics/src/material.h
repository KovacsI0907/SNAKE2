#pragma once

#include <vectors.h>

struct Material {
	vec4 kAmbient;
	vec4 kDiffuse;
	vec4 kSpecular;
	float shininess;
};