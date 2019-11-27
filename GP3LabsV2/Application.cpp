#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Common.h"
#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "CapsuleShape.h"
#include "CylinderShape.h"
#include "ConeShape.h"
#include "RigidBody.h"
#include "Lighting.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>


#include <string>

Application* Application::m_application = nullptr;
glm::vec3 colorB(0.f, 0.f, 0.f);
glm::vec3 quadPos(0.f, 0.f, -10.f);
glm::vec3 quadRot(0.f, 0.f, 0.f);
glm::vec3 quadScale(1.f, 1.f, 1.f);
bool mode1 = false;
RigidBody* r = new RigidBody();
BoxPush* bp = new BoxPush();

Application::Application() {

}

void Application::Init()
{
	//initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), errType::ERROR);
		exit(-1);

	}

	//setting openGL to v 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating a window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);

	OpenGlinit();
	GameInit();

	//std::cout << "What type of logs do you want to see? a = all, e = error, w = warning, t = trace n = none" << std::endl;
	//std::cin >> displayLogType;
}

void Application::OpenGlinit()
{
	//creating context
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();

	ImGui::CreateContext();
	ImGui_ImplSdlGL3_Init(m_window);
	// Setup style
	ImGui::StyleColorsDark();

	//Keep mouse in window
	SDL_SetWindowGrab(m_window, SDL_TRUE);

	SDL_GL_SetSwapInterval(1);

	//initialising glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);

	}

	//Smooth Shading 
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));

	//sets less or equal func for depth testing

	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enable blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);

}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;


	glm::vec3 xAxis;
	xAxis = glm::vec3(1, 0, 0);

	xAxis = glm::normalize(xAxis);
	glm::quat rot;
	float rotateA = 15.0f;
	float rotateD = -15.0f;
	
	
	
	//glm::quat result;

	while (m_appState != AppState::QUITTING)
	{
		auto currentTicks = std::chrono::high_resolution_clock::now();

		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -
			prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		SetObjTransformAttribs();
		//imgui 
		ImGui_ImplSdlGL3_NewFrame(m_window);
		//poll SDL events
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;

			case SDL_KEYDOWN:
				INPUT->GetKeyDown(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, true);

				break;
				//record when the user oresses a key
			case SDL_KEYUP:
				INPUT->GetKeyUp(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, false);
				break;

			case SDL_MOUSEMOTION:
				int oldX = x;
				int oldY = y;
				SDL_GetMouseState(&x, &y);

				if (lock) {
					if (x > oldX) {
						m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(0.05f, 0.f, 0.f));
					}
					else {
						m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(-0.05f, 0.f, 0.f));
					}
					if (y > oldY) {
						m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(0.f, -0.05f, 0.f));
					}
					else {
						m_entities.at(2)->GetTransform()->AddPosition(glm::vec3(0.f, 0.05f, 0.f));
					}
				}
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				break;
			}

			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Quit();
				break;

			case SDLK_w:
				
				Physics::GetInstance()->AddForce(deltaTime);
				
				break;

			case SDLK_s:
				
				break;

			case SDLK_a:
				rot = glm::angleAxis(glm::radians(rotateA), xAxis);
				//currentRot = m_entities.at(0)->GetTransform()->GetRotation();
				m_entities.at(0)->GetTransform()->AddRotation((rot));



				break;
			case SDLK_d:
				
				Physics::GetInstance()->AddTorque(deltaTime);

				break;

			


			}
		}

		 

		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities 
		Update(deltaTime);
		Render();

		static float f = 0.0f;
		static int counter = 0;
		ImGui::Checkbox("Mode", &mode1);
		if (mode1) {
			ImGui::Text("Hello, world!");

			ImGui::ColorEdit3("Quad color", (float*)&colorB);
			ImGui::SliderFloat3("QuadPos", (float*)&pos[entityNum], -50.0f, 50.0f);
			ImGui::SliderFloat3("QuadScale", (float*)&scaleValues[entityNum], 0.0f, 5.0f);
			ImGui::SliderFloat3("QuadRotation", (float*)&rotationValues[entityNum], 0.f, 360.0f);

			if (ImGui::Button("Rotate Object")) {
				rot = glm::angleAxis(glm::radians(rotateD), xAxis);
				m_entities.at(entityNum)->GetTransform()->AddRotation(rot);
			}


			ImGui::Checkbox("FPS camera", &lock);

			if (ImGui::Button("Button")) {                          // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				rot = glm::angleAxis(glm::radians(rotateA), xAxis);
				//currentRot = m_entities.at(0)->GetTransform()->GetRotation();
				m_entities.at(entityNum)->GetTransform()->AddRotation((rot));
			}

			if (ImGui::Button("Reset Camera")) {
				m_entities.at(2)->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 0.f));
			}
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		else {
			ImGui::Text("Hello, you are in mode 2!");

			if (ImGui::Button("Object0")) {
				entityNum = 0;
				mode1 = !mode1;
			}

			if (ImGui::Button("Object1")) {
				entityNum = 1;
				mode1 = !mode1;
			}

			if (ImGui::Button("Camera")) {
				entityNum = 2;
				mode1 = !mode1;
			}

			


		}
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(m_window);
	}


}

void Application::Quit()
{
	//Close imgui
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();
	//Close SDL 
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}

Application::~Application() {

}

Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::Update(float deltaTime)
{

	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::Render()
{
	//Clear Context
	glClearColor(0.f, 0.f, 0.4f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::GameInit()
{
	
	
	
	

	//Loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH +
		"simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");

	

	

	Entity* e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	MeshRenderer* m = e->GetComponent<MeshRenderer>();
	e->GetTransform()->SetPosition(glm::vec3(0, 0, -10));
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(
		1.f, 1.f, 1.f)));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	e->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
	pos[0] = e->GetTransform()->GetPosition();
	scaleValues[0] = e->GetTransform()->GetScale();
	glm::vec3 rotNew = glm::vec3(0, 0, 0);
	rotationValues[0] = rotNew;
	
	
	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);

	
	e->GetTransform()->SetPosition(glm::vec3(3, 0, -10));
	
	pos[1] = e->GetTransform()->GetPosition();
	scaleValues[1] = e->GetTransform()->GetScale();
	
	rotationValues[1] = rotNew;
	

	e = new Entity();
	m_entities.push_back(e);
	CameraComp* cc = new CameraComp();
	e->AddComponent(cc);
	cc->Start();
	e->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	pos[2] = e->GetTransform()->GetPosition();
	scaleValues[2] = e->GetTransform()->GetScale();
	 rotNew = glm::vec3(0, 0, 0);
	rotationValues[2] = rotNew;
	
	for (int i = 0; i < 100; i++)
	{
		e = new Entity();
		m_entities.push_back(e);
		e->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Wood.jpg"))
		);

		e->GetTransform()->SetPosition(glm::vec3(0, i + 1.f, -10));
		e->AddComponent<RigidBody>();
		e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		
		
			e->AddComponent<BoxPush>();
			e->GetComponent<BoxPush>()->Init();
		
		//e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		e->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));

	}
	
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

void Application::SetObjTransformAttribs()
{
	//if (entityNum == 2 && lock == false) {
		//Position
		if (m_entities.at(entityNum)->GetTransform()->GetPosition() != pos[entityNum]) {
			m_entities.at(entityNum)->GetTransform()->SetPosition(pos[entityNum]);
		}
	//}
	
	//Scale
	if (m_entities.at(entityNum)->GetTransform()->GetScale() != scaleValues[entityNum]) {
		m_entities.at(entityNum)->GetTransform()->SetScale(scaleValues[entityNum]);
	}

	
	//Rotation
	glm::quat rotX = glm::angleAxis(glm::radians(rotationValues[entityNum].x), glm::vec3(1.f, 0.f, 0.f));
	glm::quat rotY = glm::angleAxis(glm::radians(rotationValues[entityNum].y), glm::vec3(0.f, 1.f, 0.f));
	glm::quat rotZ = glm::angleAxis(glm::radians(rotationValues[entityNum].z), glm::vec3(0.f, 0.f, 1.f));
	glm::quat rotTotal = rotX * rotY * rotZ;
	


	if (m_entities.at(entityNum)->GetTransform()->GetRotation() != rotTotal)
	{
		m_entities.at(entityNum)->GetTransform()->SetRotation(rotTotal);
	}
	
}
	

	
	
