#pragma once

#include "nfpch.h"

#include "Core/Log.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "Input/Event.h"
#include "Input/MouseCodes.h"
#include "Input/MouseEvent.h"
#include "Input/KeyCodes.h"
#include "Input/KeyEvent.h"

#include "ImGui/ImGuiLayer.h"

namespace Nifty {

	class EngineLayer;

	class Application
	{
	private:
		Window m_Window = Window(WindowProps());
		Window m_Viewport = Window(WindowProps());
		ImGuiLayer* m_ImGuiLayer;
		EngineLayer* m_EngineLayer;
		LayerStack m_LayerStack;

		bool running = true;

	public:
		float deltaTime = 0.0f;
		float lastFrameTime = 0.0f;

	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void mousebuttoncallback(GLFWwindow* window, int button, int action, int mods);
		void mousecallback(GLFWwindow* window, double xposin, double yposin);
		void scrollcallback(GLFWwindow* window, double xoffset, double yoffset);

		static void keycallback_dispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (s_Instance)
				s_Instance->keycallback(window, key, scancode, action, mods);
		}
		static void mousebuttoncallback_dispatch(GLFWwindow* window, int key, int action, int mods)
		{
			if (s_Instance)
				s_Instance->mousebuttoncallback(window, key, action, mods);
		}
		static void mousecallback_dispatch(GLFWwindow* window, double xposin, double yposin)
		{
			if (s_Instance)
				s_Instance->mousecallback(window, xposin, yposin);
		}
		static void scrollcallback_dispatch(GLFWwindow* window, double xoffset, double yoffset)
		{
			if (s_Instance)
				s_Instance->scrollcallback(window, xoffset, yoffset);
		}

		static Application& Get() { return *s_Instance; }

		Window& GetWindow() { return m_Window; }
		Window& GetViewport() { return m_Viewport; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		EngineLayer* GetEngineLayer() { return m_EngineLayer; }

		void Run();

		static Application* s_Instance;
	};

	Application* CreateApplication();
}