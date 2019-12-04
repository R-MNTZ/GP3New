#include "pch.h"
#include "Input.h"
#include "Entity.h"
#include "Application.h"
#include "Physics.h"

Input* Input::m_instance = nullptr;

Input::Input()
{
}

Input* Input::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Input();
	}
	return m_instance;
}

void Input::SetKey(SDL_Keycode key, bool state)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}
	if (index < m_state.keys.size())
	{
		m_state.keys[index] = state;
	}
}

bool Input::GetKey(SDL_Keycode key)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}

	if (index < m_state.keys.size())
	{
		return m_state.keys[index];
	}
	else return false;
}

bool Input::GetKeyDown(SDL_Keycode key)
{
	if (key != NULL) 
	{
		switch (Application::event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Application::GetInstance()->Quit();
			break;

		case SDLK_w:

			Physics::GetInstance()->AddForce();

			break;

		case SDLK_s:
			Physics::GetInstance()->SubForce();
			break;

		case SDLK_a:

			Physics::GetInstance()->AddTorque2();


			break;
		case SDLK_d:

			Physics::GetInstance()->AddTorque();

			break;




		}
		return true;
	}
	else
	{
		return false;
	}
	
	return false;
}

bool Input::GetKeyUp(SDL_Keycode key)
{
	if (key != NULL)
	{
		LOG_DEBUG("key up", logType::TRACE);
		return true;
	}
	else
	{
		return false;
	}
	
	return false;
}

void Input::Mouse() {
	int oldX = Application::GetInstance()->x;
	int oldY = Application::GetInstance()->y;
	SDL_GetMouseState(&Application::GetInstance()->x, &Application::GetInstance()->y);

	if (Application::GetInstance()->lock) {
		if (Application::GetInstance()->x > oldX) {
			Application::m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(0.1f, 0.f, 0.f));
		}
		else {
			Application::m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(-0.1f, 0.f, 0.f));
		}
		if (Application::GetInstance()->y > oldY) {
			Application::m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(0.f, -0.1f, 0.f));
		}
		else {
			Application::m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(0.f, 0.1f, 0.f));
		}
	}
	MoveMouse(glm::ivec2(Application::event.motion.xrel, Application::event.motion.yrel));
}

void Input::ControllerBtn() {
	switch (Application::event.cbutton.button) {
		
	case SDL_CONTROLLER_BUTTON_A:
		std::cout << "a" << std::endl;
		break;

	
	case SDL_CONTROLLER_BUTTON_B:
		std::cout << "b" << std::endl;
		break;

	case SDL_CONTROLLER_BUTTON_X:
		std::cout << "x" << std::endl;
		break;

	case SDL_CONTROLLER_BUTTON_Y:
		std::cout << "y" << std::endl;
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		Physics::GetInstance()->AddForce();
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		Physics::GetInstance()->SubForce();
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		Physics::GetInstance()->AddTorque();
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		Physics::GetInstance()->AddTorque2();
		break;
	}
}

void Input::ControllerAxis()
{
	switch (Application::event.jaxis.axis) {

		//x axis
	case 0:
		if (Application::event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
			Physics::GetInstance()->AddTorque2();
		}
		else if (Application::event.jaxis.value > JOYSTICK_DEAD_ZONE) {
			Physics::GetInstance()->AddTorque();
		}

		break;

		//y axis
	case 1:
		if (Application::event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
			Physics::GetInstance()->AddForce();
		}
		else if (Application::event.jaxis.value > JOYSTICK_DEAD_ZONE) {
			Physics::GetInstance()->SubForce();
		}
		break;
	}
}





