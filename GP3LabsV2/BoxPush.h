#pragma once
#include "Component.h"
#include "Physics.h"
#include "CollisionShape.h"
#include "pch.h"

class BoxPush : public Component
{
public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnStart() override;


	void Init();
};