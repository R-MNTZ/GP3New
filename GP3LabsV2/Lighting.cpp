#include "Lighting.h"

Lighting* Lighting::m_lighting = nullptr;

void Lighting::passLightUniform()
{
	
	sp = Resources::GetInstance()->GetShader("simple");
	GLuint loc = glGetUniformLocation(sp->Get(), "lightPos");
	glUniform3f(loc, lightPosition.x, lightPosition.y, lightPosition.z);
	 loc = glGetUniformLocation(sp->Get(), "lightColor");
	glUniform3f(loc, lightColor.x, lightColor.y, lightColor.z);
}

Lighting* Lighting::GetInstance()
{
	if (m_lighting == nullptr)
	{
		m_lighting = new Lighting();
	}
	return m_lighting;
}

Lighting::~Lighting() {
	delete m_lighting;
}
