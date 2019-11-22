#pragma once
#include "pch.h"
#include "RigidBody.h"
#include "Physics.h"
#include "Entity.h"

void RigidBody::OnUpdate(float deltaTime)
{
}

void RigidBody::OnRender()
{
}

void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3 localInteria)
{
	btTransform bT =
		Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localInteria.x, localInteria.y, localInteria.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState,
		m_shape->Get(), li);
	Physics::GetInstance()->AddRigidBody(this);
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);


}

void RigidBody::UpdateParent()
{
	
	
	//TODO Get transform from rigidBody's motionstate
	//and apply it to the game object's transform
	btTransform transform;
	
	m_rigidBody->getMotionState()->getWorldTransform(transform);

	btQuaternion rot = transform.getRotation();
	//glm::quat otherRot = glm::quat(rot.x, rot.y, rot.z, rot.w);
	btVector3 pos = transform.getOrigin();
	//glm::vec3 otherPos = glm::vec3(pos.x, pos.y, pos.z);
	
	m_entity->GetTransform()->SetRotation(glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
	m_entity->GetTransform()->SetPosition(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
	

	
}

void RigidBody::UpdateRigidBody()
{
	btTransform t =
		Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	
	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);

}
