#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

class MeshRenderer : public Component
{
	std::shared_ptr<Texture> m_texture;
	
	std::shared_ptr<Model> m_model;
	std::shared_ptr<ShaderProgram> m_program;

	int* a;


public:
	//Inherited via component
	MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	~MeshRenderer();
	virtual void OnUpdate(float delatTime) override;
	virtual void OnRender() override;
	virtual void OnStart() override;

};
