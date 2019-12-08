#pragma once
#include "pch.h"
#include "CollisionShape.h"

class SphereShape : public CollisionShape
{
private:
	float m_scalar;

public:
	SphereShape(float scalar);
	~SphereShape();
};
