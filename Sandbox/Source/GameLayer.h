#pragma once

#include "Nifty.h"

using namespace Nifty;

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:
	EngineLayer* m_EngineLayer;
};