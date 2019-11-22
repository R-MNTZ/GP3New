#pragma once
//forward declaring to avoid circular includes

class Entity;
class Component
{
protected:
	Entity* m_entity = nullptr;
	friend class Entity;

public:
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnRender()
	{};
	virtual void OnStart() {};
};
