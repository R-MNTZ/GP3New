#pragma once
#include "pch.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "Resources.h"
#include "Common.h"

class Lighting
{
private:
	
	std::shared_ptr<ShaderProgram> sp;
	static Lighting* m_lighting;
	~Lighting();
	
	
public:
	void passLightUniform();
	static Lighting* GetInstance();
	

};