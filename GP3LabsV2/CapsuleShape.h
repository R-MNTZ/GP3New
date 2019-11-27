#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CapsuleShape : public CollisionShape
{
private:
	float m_radius;
	float m_height;

public:
	CapsuleShape(float radius, float height);
};
