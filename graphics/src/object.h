#pragma once

#include<material.h>
#include<geometry.h>
#include<vectors.h>
#include<vector>
#include<shaderprogram.h>
#include<memory>

/// <summary>
/// A generic object to be used in the game world.
/// TAKES OWNERSHIP of geometry.
/// </summary>
/// <remarks>
/// Has a transform (position, scale, rotation).
/// Can have a geometry.
/// Must have a material if it has a geometry.
/// Can have a parent. 
/// Transforms from parents are applied to children.
/// </remarks>
class Object {
protected:

	std::unique_ptr<Geometry> geometry = nullptr;
	Object* parent = nullptr;

	/// <returns>The modelling transformation matrix of the object without the parent transforms</returns>
	mat4 ownModel();
	/// <returns>The inverse of the modelling transformation matrix</returns>
	mat4 ownModelInverse();
	
public:
	Material material;
	vec3 position = vec3(0, 0, 0);
	vec3 scale = vec3(1, 1, 1);
	float rotAngle = 0;
	vec3 rotAxis = vec3(0, 0, 1);

	Object() {};

	Object(std::unique_ptr<Geometry> geometryPtr, Material material) {
		geometry = std::move(geometryPtr);
		if (!geometry->created())
			geometry->create();
		this->material = material;
	};

	/// <summary>
	/// Changes the geometry of the object.
	/// </summary>
	void setGeometry(std::unique_ptr<Geometry> geometry);
	
	/// <summary>
	/// Draws the geometry of the object with it's material.
	/// </summary>
	void draw();

	/// <returns>The modeling transformation matrix with the parent transforms applied.</returns>
	mat4 getModel();

	/// <returns>The inverse of the modeling transformation matrix with the parent transforms applied</returns>
	mat4 getModelInverse();

	/// <summary>
	/// Changes the objects parent
	/// </summary>
	void setParent(Object* parent);
};