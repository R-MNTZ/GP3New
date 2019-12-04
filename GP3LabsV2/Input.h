#pragma once
#include "Common.h"
#include "pch.h"
#include "InputState.h"
#define INPUT Input::GetInstance()

class Input 
{
private:
	Input();
	static Input* m_instance;
	InputState m_state;
	const int JOYSTICK_DEAD_ZONE = 3000;


public:
	static Input* GetInstance();
	void SetKey(SDL_Keycode key, bool state);
	bool GetKey(SDL_Keycode key); //check if its curently down or not
	bool GetKeyDown(SDL_Keycode key);
	bool GetKeyUp(SDL_Keycode key);
	

	glm::ivec2 GetMousePos() 
	{ 
		
		
		return m_state.mousePos;
	}
	void MoveMouse(glm::ivec2 delta) { m_state.mousePos += delta; }

	void Mouse();
	void ControllerBtn();
	void ControllerAxis();

};
