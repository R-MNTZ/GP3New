#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"
#include "CameraComp.h"



MeshRenderer::MeshRenderer(Model* model, ShaderProgram* program, Texture* texture)
{
	m_model = model;
	
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float delatTime)
{
	//LOG_DEBUG("Im updating!", errType::TRACE);
}

void MeshRenderer::OnRender()
{
	m_program->Use();

	//set uniforms here!
	glm::mat4 model =
		m_entity->GetTransform()->GetTransformationMatrix();
	//	glm::mat4 view = glm::lookAt(glm::vec3(0.f,0.f,0.f), glm::vec3(0.f, 0.f, 1.f)
		//	- glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)
			//);

	//	glm::mat4 proj = glm::perspective(45.5f, (float)WINDOW_W /
			//(float)WINDOW_H, 0.1f, 1000.f);

		//glm::mat4 mvp = proj * view * model;
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



	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();

		if (m_texture) m_texture->Bind();

		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(),GL_UNSIGNED_INT, 0));
	
	}

	
	

}

void MeshRenderer::OnStart()
{
	/*
	CameraComp a;
	a.m_camera = new Camera(m_entity->GetTransform());

	if (Application::GetInstance()->GetCamera() == nullptr)
	{
		Application::GetInstance()->SetCamera(a.m_camera);
	}
	*/
}

