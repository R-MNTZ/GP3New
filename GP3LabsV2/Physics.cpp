#include "pch.h"
#include "Physics.h"
#include "RigidBody.h"

Physics* Physics::m_instance = nullptr;

Physics::Physics()
{
	Init();
}

void Physics::Init()
{
	m_world->setGravity(btVector3(0, -9.81, 0));
}

void Physics::AddRigidBody(RigidBody* r)
{
	m_rbodies.push_back(r);
	m_world->addRigidBody(r->Get());
}

void Physics::PreUpdate()
{
	for (auto r : m_rbodies)
	{
		r->UpdateRigidBody();
	}
}