#include "nfpch.h"
#include "Application.h"
#include "Engine/EngineLayer.h"

namespace Nifty {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = Window(WindowProps(1800, 1000, 0, 0, "Nifty Engine"));
		m_Viewport = Window(WindowProps(1550, 800, 250, 200, "Viewport"));

		m_ImGuiLayer = new ImGuiLayer();
		m_EngineLayer = new EngineLayer();
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::Run()
	{
		std::cout << "Current project path is: " << std::filesystem::current_path() << std::endl;

		m_Window.InitOpenGL();

		glfwSetKeyCallback(m_Window.GetNativeWindow(), keycallback_dispatch);
		glfwSetMouseButtonCallback(m_Window.GetNativeWindow(), mousebuttoncallback_dispatch);
		glfwSetCursorPosCallback(m_Window.GetNativeWindow(), mousecallback_dispatch);
		glfwSetScrollCallback(m_Window.GetNativeWindow(), scrollcallback_dispatch);

		m_ImGuiLayer->OnAttach();
		m_EngineLayer->OnAttach();
		for (Layer* layer : m_LayerStack)
			layer->OnAttach();

		while (!glfwWindowShouldClose(m_Window.GetNativeWindow()))
		{
			float currentFrameTime = static_cast<float>(glfwGetTime());
			deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			m_EngineLayer->OnUpdate();
			if (gameRunning)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			m_EngineLayer->OnImGuiRender();
			if (gameRunning)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			glfwSwapBuffers(m_Window.GetNativeWindow());
			glfwPollEvents();
		}

		for (Layer* layer : m_LayerStack)
			layer->OnDetach();
		m_EngineLayer->OnDetach();
		m_ImGuiLayer->OnDetach();
		glfwTerminate();
	}

	void Application::keycallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			KeyPressedEvent e(keycode);

			m_ImGuiLayer->OnEvent(e);
			m_EngineLayer->OnEvent(e);
			for (Layer* layer : m_LayerStack)
				layer->OnEvent(e);
		}
		else if (action == GLFW_RELEASE)
		{
			KeyReleasedEvent e(keycode);

			m_ImGuiLayer->OnEvent(e);
			m_EngineLayer->OnEvent(e);
			for (Layer* layer : m_LayerStack)
				layer->OnEvent(e);
		}
	}

	void Application::mousebuttoncallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			MouseButtonPressedEvent e(button);

			m_ImGuiLayer->OnEvent(e);
			m_EngineLayer->OnEvent(e);
			for (Layer* layer : m_LayerStack)
				layer->OnEvent(e);
		}
		else if (action == GLFW_RELEASE)
		{
			MouseButtonReleasedEvent e(button);

			m_ImGuiLayer->OnEvent(e);
			m_EngineLayer->OnEvent(e);
			for (Layer* layer : m_LayerStack)
				layer->OnEvent(e);
		}
	}

	void Application::mousecallback(GLFWwindow* window, double xposin, double yposin)
	{
		MouseMovedEvent e(xposin, yposin);

		m_ImGuiLayer->OnEvent(e);
		m_EngineLayer->OnEvent(e);
		for (Layer* layer : m_LayerStack)
			layer->OnEvent(e);
	}

	void Application::scrollcallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		MouseScrolledEvent e(xoffset, yoffset);

		m_ImGuiLayer->OnEvent(e);
		m_EngineLayer->OnEvent(e);
		for (Layer* layer : m_LayerStack)
			layer->OnEvent(e);
	}
}