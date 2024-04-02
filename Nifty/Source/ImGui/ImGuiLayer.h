#pragma once

#include "nfpch.h"
#include "Core/Layer.h"

namespace Nifty {

	class ImGuiLayer : public Layer
	{
	private:
		bool m_BlockEvents = false;

	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetCustomStyle();
	};
}