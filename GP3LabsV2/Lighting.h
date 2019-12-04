#pragma once
#include "pch.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "Resources.h"

class Lighting
{
private:
	glm::vec3 light = glm::vec3(0.0f, 10.0f, 0.0f);
	std::shared_ptr<ShaderProgram> sp;
public:
	void passLightUniform() {
		sp = Resources::GetInstance()->GetShader("simple");
		GLuint loc = glGetUniformLocation( sp->Get(), "lightPos");
		glUniform3f(loc, light.x, light.y, light.z);
	}

};