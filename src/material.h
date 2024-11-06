#pragma once

#include <vectors.h>
#include <texture.h>

/// <summary>
/// Stores information about materials. (Phong-Blinn model)
/// </summary>
struct Material {
	vec4 kAmbient;
	vec4 kDiffuse;
	vec4 kSpecular;
	float shininess;

	Texture* texture = nullptr;
};