#include "pch.h"
#include "SphereShape.h"

SphereShape::SphereShape(float scalar) {
	m_scalar = scalar;
	m_shape = new btSphereShape(scalar);
	m_type = CShapeType::SPHERE;
}

SphereShape::~SphereShape() {
	delete m_shape;
}
