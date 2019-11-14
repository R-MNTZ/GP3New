#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"

#include <string>

Application* Application::m_application = nullptr;

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
	xAxis = glm::vec3(0, 1, 0);

	xAxis = glm::normalize(xAxis);
	glm::quat rot;
	float rotateA = 15.0f;
	float rotateD = -15.0f;
	glm::quat currentRot;
	//glm::quat result;

	while (m_appState != AppState::QUITTING)
	{
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
			}

			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Quit();
				break;

			case SDLK_w:
				m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0.f, 2.f,0.f));
				break;

			case SDLK_s:
				m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0.f, -2.f, 0.f));
				break;

			case SDLK_a:
				rot = glm::angleAxis(glm::radians(rotateA), xAxis);
				//currentRot = m_entities.at(0)->GetTransform()->GetRotation();
				m_entities.at(0)->GetTransform()->AddRotation((rot));



				break;
			case SDLK_d:
				rot = glm::angleAxis(glm::radians(rotateD), xAxis);
				m_entities.at(0)->GetTransform()->AddRotation(rot);

				break;

			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				break;


			}
		}

		auto currentTicks = std::chrono::high_resolution_clock::now();

		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -
			prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		//update and render all entities 
		Update(deltaTime);
		Render();

		//LOG_DEBUG(std::to_string(deltaTime), errType::TRACE);
		//LOG_DEBUG("err", errType::ERROR);
		//LOG_DEBUG("warning", errType::WARNING);

		SDL_GL_SwapWindow(m_window);
	}


}

void Application::Quit()
{
	//Close SDL 
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
	//improve this


	//Loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH +
		"simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");

	m_entities.at(0)->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	

	m_entities.at(0)->GetTransform()->SetPosition(glm::vec3(0, 0, -10));
	

	m_entities.push_back(new Entity());
	CameraComp* cc = new CameraComp();
	m_entities.at(1)->AddComponent(cc);
	cc->Start();

}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}
