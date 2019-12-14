#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"
#include "CameraComp.h"
#include "Lighting.h"



MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_model = model;
	
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float delatTime)
{
	//LOG_DEBUG("Update() is working!!", errType::TRACE);
}

void MeshRenderer::OnRender()
{
	m_program->Use();

	
	glm::mat4 model =
		m_entity->GetTransform()->GetTransformationMatrix();
	
	GLuint loc = glGetUniformLocation(m_program->Get(), "model");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));
	glm::vec3 viewPos = Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition();
	loc = glGetUniformLocation(m_program->Get(), "viewPos");
	glUniform3f(loc, viewPos.x, viewPos.y, viewPos.z);
	glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;
	 loc = glGetUniformLocation(m_program->Get(), "MVP");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));
	glm::vec3 oColor = glm::vec3(colorB.x, colorB.y, colorB.z);
	loc = glGetUniformLocation(m_program->Get(), "objectColor");
	glUniform3f(loc, oColor.x, oColor.y, oColor.z);
	GLint applyGamma = gamma;
	loc = glGetUniformLocation(m_program->Get(), "gamma");
	glUniform1i(loc, applyGamma);
	//Pass in Light values
	Lighting::GetInstance()->passLightUniform();
	


	

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();

		if (m_texture) m_texture->Bind();

		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(),GL_UNSIGNED_INT, 0));
	
	}

	
	

}

void MeshRenderer::OnStart()
{
	
}

MeshRenderer::~MeshRenderer() {
	
}

