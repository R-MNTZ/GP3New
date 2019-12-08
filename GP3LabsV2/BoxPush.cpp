#include "BoxPush.h"

void BoxPush::OnUpdate(float deltaTime)
{

}

void BoxPush::OnRender()
{
}

void BoxPush::OnStart()
{
}

void BoxPush::Init() {
	Physics::GetInstance()->AddBoxPush(this);
}
