#pragma once

#include "nfpch.h"
#include "Input/Event.h"

namespace Nifty {

	class Layer
	{
	protected:
		std::string m_DebugName;

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}

		virtual void OnEvent(Event& e) {}

		const std::string& GetName() const { return m_DebugName; }
	};
}