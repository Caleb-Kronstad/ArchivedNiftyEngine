#pragma once

#include "nfpch.h"

namespace Nifty {

	struct WindowProps {
		unsigned int Width;
		unsigned int Height;
		unsigned int xOffset;
		unsigned int yOffset;
		std::string Title;

		WindowProps(const unsigned int& width = 1280, const unsigned int& height = 720, const unsigned int& xoffset = 0, unsigned int yoffset = 0, const std::string& title = "Nifty Engine")
			: Width(width), Height(height), xOffset(xoffset), yOffset(yoffset), Title(title)
		{
		}
	};

	class Window
	{
	private:
		GLFWwindow* m_Window;
		WindowProps m_WindowProps = WindowProps();
		bool m_IsVSync = false;

	public:
		Window(WindowProps windowProps) : m_WindowProps(windowProps)
		{
		}
		virtual ~Window() = default;

		void InitOpenGL();

		unsigned int& GetWidth() { return m_WindowProps.Width; }
		unsigned int& GetHeight() { return m_WindowProps.Height; }
		unsigned int& GetXOffset() { return m_WindowProps.xOffset; }
		unsigned int& GetYOffset() { return m_WindowProps.yOffset; }
		std::string& GetTitle() { return m_WindowProps.Title; }
		WindowProps& GetProps() { return m_WindowProps; }

		void ChangeProps(WindowProps props) { m_WindowProps = props; }

		GLFWwindow* GetNativeWindow() { return m_Window; }
		void ChangeNativeWindow(GLFWwindow* window) { m_Window = window; }

		void SetVSync(bool enabled) { m_IsVSync = enabled; }
		bool IsVSync() const { return m_IsVSync; }
	};
}