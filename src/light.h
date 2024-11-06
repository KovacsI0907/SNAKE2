#pragma once

#include <vectors.h>

/// <summary>
/// Stores basic information about a light source.
/// </summary>
/// <remarks>
/// Position is expected to be given in homogeneous coordinates.
/// If the position is an ideal point, then the light source is considered a directional light, otherwise it's considered to be a point light.
/// Intensity should be given for the r,g,b wavelengths, 4th coordinate is the alpha value.
/// </remarks>
struct Light {
	vec4 position;
	vec4 intensity;
};