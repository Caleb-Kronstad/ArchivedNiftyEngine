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
	std::vector<Model*>*& models = m_EngineLayer->GetModels();
	std::vector<Entity*>*& entities = m_EngineLayer->GetEntities();
}

void GameLayer::OnEvent(Event& e)
{

}

void GameLayer::OnImGuiRender()
{

}