#pragma once
#include "pch.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "Resources.h"
#include "Common.h"

class Lighting
{
private:
	glm::vec3 light = glm::vec3(0.0f, 10.0f, 0.0f);
	std::shared_ptr<ShaderProgram> sp;
	static Lighting* m_lighting;
	~Lighting();
	
	
public:
	void passLightUniform();
	static Lighting* GetInstance();
	

};