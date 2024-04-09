#include "GameLayer.h"

GameLayer::GameLayer() : Layer("Sandbox")
{

}
GameLayer::~GameLayer()
{

}

void GameLayer::OnAttach()
{
	Application& app = Application::Get();
	Window& window = app.GetWindow();
	Window& viewport = app.GetViewport();
	m_EngineLayer = app.GetEngineLayer();
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate()
{
	std::vector<Shader*>& shadersRef = *m_EngineLayer->m_Shaders;
	std::vector<Model*>& modelsRef = *m_EngineLayer->m_Models;
	std::vector<GameObject*>& objectsRef = *m_EngineLayer->m_GameObjects;


}

void GameLayer::OnEvent(Event& e)
{

}

void GameLayer::OnImGuiRender()
{

}