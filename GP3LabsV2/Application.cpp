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
int gamma = 1;
SDL_Event Application::event;
std::vector<Entity*> Application::m_entities;


Application::Application() {

}



void Application::Init()
{
	//initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), logType::ERROR);
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
	gController = nullptr;
	controller = nullptr;
	controllerHaptic = nullptr;

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		controller = SDL_JoystickOpen(i);
		
		LOG_DEBUG("CONTROLLER CONNECTED", logType::TRACE);

		controllerHaptic = SDL_HapticOpenFromJoystick(controller);
		SDL_HapticRumbleInit(controllerHaptic);
		break;
	}

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		gController = SDL_GameControllerOpen(i);
		
		LOG_DEBUG("CONTROLLER CONNECTED", logType::TRACE);
		break;
	}


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
	//GL_ATTEMPT(glEnable(GL_CULL_FACE));
	//GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);

}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	
	//glm::quat result;

	while (m_appState != AppState::QUITTING)
	{

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

			case SDL_CONTROLLERBUTTONDOWN:
			
				INPUT->ControllerBtn();
				break;

			
				
			
		
			case SDL_CONTROLLERAXISMOTION:
				
				INPUT->ControllerAxis();
				break;

			case SDL_KEYDOWN:
				INPUT->GetKeyDown(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, true);

				break;

				//record when the user presses a key
			case SDL_KEYUP:
				INPUT->GetKeyUp(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, false);
				break;

			case SDL_MOUSEMOTION:
				INPUT->Mouse();
				break;
			}

			
		}
		UserInterface();
		auto currentTicks = std::chrono::high_resolution_clock::now();

		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -
			prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities 
		Update(deltaTime);
		Render();



		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(m_window);
	}


}

void Application::Quit()
{
	//Close Controller & Joystick & Haptic
	SDL_JoystickClose(controller);
	SDL_GameControllerClose(gController);
	SDL_HapticClose(controllerHaptic);
	controller, gController, controllerHaptic = NULL;

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

	delete[] pos;
	delete[] scaleValues;
	delete[] rotationValues;
	delete m_application;
	
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
	Resources::GetInstance()->AddModel("monkey3.obj");
	Resources::GetInstance()->AddModel("heart.obj");
	Resources::GetInstance()->AddModel("lego.obj");
	Resources::GetInstance()->AddModel("star.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddTexture("brick.jpg");
	Resources::GetInstance()->AddTexture("sky.jpg");
	Resources::GetInstance()->AddTexture("lava.jpg");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH +
		"simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");
	





	Entity* e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("star.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("lava.jpg"))
	);
	MeshRenderer* m = e->GetComponent<MeshRenderer>();
	e->GetTransform()->SetPosition(glm::vec3(0, 0, -10));
	e->GetTransform()->SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(
		1.f, 0.5f, 1.f)));

	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	pos[0] = e->GetTransform()->GetPosition();
	scaleValues[0] = e->GetTransform()->GetScale();
	glm::vec3 rotNew = glm::vec3(0, 0, 0);
	rotationValues[0] = rotNew;


	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("heart.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);


	e->GetTransform()->SetPosition(glm::vec3(3, 0, -10));
	e->GetTransform()->SetScale(glm::vec3(0.05f, 0.05f, 0.05f));

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

	for (int i = 0; i < 10; i++)
	{
		e = new Entity();
		m_entities.push_back(e);
		if (i == 8) {
			e->AddComponent(
				new MeshRenderer(
					Resources::GetInstance()->GetModel("monkey3.obj"),
					Resources::GetInstance()->GetShader("simple"),
					Resources::GetInstance()->GetTexture("Wood.jpg"))
			);
		}
		else if (i == 9) {
			e->AddComponent(
				new MeshRenderer(
					Resources::GetInstance()->GetModel("lego.obj"),
					Resources::GetInstance()->GetShader("simple"),
					Resources::GetInstance()->GetTexture("Wood.jpg"))
			);
		}
		else {
			e->AddComponent(
				new MeshRenderer(
					Resources::GetInstance()->GetModel("cube.obj"),
					Resources::GetInstance()->GetShader("simple"),
					Resources::GetInstance()->GetTexture("Wood.jpg"))
			);
		}
		

		e->GetTransform()->SetPosition(glm::vec3(0, i + 10.f, -10));
		e->AddComponent<RigidBody>();
		e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));


		e->AddComponent<BoxPush>();
		e->GetComponent<BoxPush>()->Init();

		//e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		e->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));

		pos[i + 3] = e->GetTransform()->GetPosition();
		scaleValues[i + 3] = e->GetTransform()->GetScale();
		glm::vec3 rotNew = glm::vec3(0, 0, 0);
		rotationValues[i + 3] = rotNew;


	}

	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	m = e->GetComponent<MeshRenderer>();
	e->GetTransform()->SetPosition(glm::vec3(0, -10, -10));
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(
		100.f, 1.f, 100.f)));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	e->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));
	

	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("sky.jpg"))
	);
	m = e->GetComponent<MeshRenderer>();
	e->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	
	e->GetTransform()->SetScale(glm::vec3(500.f, 500.f, 500.f));

	e = new Entity();
	delete e;


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
	
	if (entityNum > 2) {
		if (m_entities.at(entityNum)->GetTransform()->GetPosition() != pos[entityNum]) {
			m_entities.at(entityNum)->GetTransform()->SetPosition(pos[entityNum]);
		}
		pos[entityNum] = m_entities.at(entityNum)->GetTransform()->GetPosition();

	}
	else {


		// position
		if (m_entities.at(entityNum)->GetTransform()->GetPosition() != pos[entityNum]) {
			m_entities.at(entityNum)->GetTransform()->SetPosition(pos[entityNum]);
		}
		pos[entityNum] = m_entities.at(entityNum)->GetTransform()->GetPosition();
		
	}

	for (int i = 3; i < 13; i++) {
		pos[i] = m_entities.at(i)->GetTransform()->GetPosition();
	}


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

void Application::UserInterface() {
	if (ImGui::Button("Select Component")) {
		m_uiState = UiState::SELECT;
	}
	ImGui::SameLine();
	if (ImGui::Button("Edit Component")) {
		m_uiState = UiState::EDITCOMP;
	}
	ImGui::SameLine();
	if (ImGui::Button("Edit all Components")) {
		m_uiState = UiState::EDITALL;
	}


	if (m_uiState == UiState::EDITCOMP) {
		ImGui::Text("Hello, world!");


		ImGui::SliderFloat3("Position", (float*)&pos[entityNum], -50.0f, 50.0f);
		ImGui::SliderFloat3("Scale", (float*)&scaleValues[entityNum], 0.0f, 5.0f);
		ImGui::SliderFloat3("Rotation", (float*)&rotationValues[entityNum], 0.f, 360.0f);



		if (entityNum != 2) {
			ImGui::Checkbox("FPS camera", &lock);
		}

		if (entityNum > 2) {
			if (ImGui::Button("De-Activte Physics")) {                          // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				m_entities.at(entityNum)->GetComponent<RigidBody>()->StopMovement();
			}

			if (ImGui::Button("Re-Activte Physics")) {
				m_entities.at(entityNum)->GetComponent<RigidBody>()->ResumeMovement();
			}


		}
		if (ImGui::Button("Reset Camera")) {
			m_entities.at(2)->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 0.f));

		}




	}
	else if (m_uiState == UiState::SELECT) {




		if (ImGui::Button("Object0")) {
			entityNum = 0;
			m_uiState = UiState::EDITCOMP;
		}

		if (ImGui::Button("Object1")) {
			entityNum = 1;
			m_uiState = UiState::EDITCOMP;
		}

		if (ImGui::Button("Camera")) {
			entityNum = 2;
			m_uiState = UiState::EDITCOMP;
		}
		for (int i = 3; i < 13; i++) {
			int objNum = i - 1;
			std::string a = "Object ";
			a.append(std::to_string(i - 1));
			char chararray[10];
			strcpy(chararray, a.c_str());
			if (ImGui::Button(chararray)) {
				entityNum = i;
				m_uiState = UiState::EDITCOMP;
			}
		}


	}
	else if (m_uiState == UiState::EDITALL) {
		ImGui::ColorEdit3("Box color", (float*)&colorB);
		if (ImGui::Button("Toggle Gamma Correction"))
		{
			if (gamma == 1) {
				gamma = 0;
			}
			else if (gamma == 0) {
				gamma = 1;
			}
		}

		ImGui::Text("Skybox");
		if (ImGui::Button("Brick Wall")) {
			m_entities.at(14)->AddComponent(new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("brick.jpg"))
			);
		}
		ImGui::SameLine();
		if (ImGui::Button("Sky")) {
			m_entities.at(14)->AddComponent(new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("sky.jpg"))
			);
		}
		ImGui::SameLine();
		if (ImGui::Button("Wood")) {
			m_entities.at(14)->AddComponent(new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Wood.jpg"))
			);
		}
		ImGui::SameLine();
		if (ImGui::Button("Lava")) {
			m_entities.at(14)->AddComponent(new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("lava.jpg"))
			);
		}
	}
}


