#include "Lighting.h"

Lighting* Lighting::m_lighting = nullptr;

void Lighting::passLightUniform()
{
	light.y = -sinf(counter) * 10;
	std::cout << light.y << std::endl;
	sp = Resources::GetInstance()->GetShader("simple");
	GLuint loc = glGetUniformLocation(sp->Get(), "lightPos");
	glUniform3f(loc, light.x, light.y, light.z);
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
