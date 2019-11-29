#pragma once
#include "pch.h"
#include "Entity.h"

#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

enum AppState
{
	INITILISING, RUNNING, QUITTING
};

enum UiState
{
	SELECT, EDITCOMP, EDITALL
};

class Camera; //forward declaration

class Application
{
private:
	//private variables
	static Application* m_application;
	SDL_Window* m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	int m_windowWidth = 1280;
	int m_windowHeight = 720;
	AppState m_appState = AppState::INITILISING;
	UiState m_uiState = UiState::SELECT;
	float m_worldDeltaTime = 0.f;
	std::vector<Entity*> m_entities;
	Camera* m_mainCamera = nullptr;

	//Scene graph variables
	
	int x, y;
	bool lock = false;
	int currentObj = 0;
	glm::vec3 pos[5];
	glm::vec3 scaleValues[5];
	glm::vec3 rotationValues [5];
	int entityNum;

	//private functions
	Application();
	void Init();
	void OpenGlinit();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();

	void GameInit();
	void SetObjTransformAttribs();




public:
	//public functions
	~Application();
	static Application* GetInstance();
	void Run();

	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowWidth() { return m_windowWidth; }

	inline Camera* GetCamera() { return m_mainCamera; }
	void SetCamera(Camera* camera);
	




};