#include "nfpch.h"
#include "Window.h"

namespace Nifty {
	void Window::InitOpenGL()
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		m_Window = glfwCreateWindow(GetWidth(), GetHeight(), GetTitle().c_str(), NULL, NULL);

		if (m_Window == NULL) {
			std::cout << "Error creating GLFW window" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Error initializing GLAD" << std::endl;
		}

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		//glEnable(GL_STENCIL_TEST);

		glViewport(0, 0, GetWidth(), GetHeight());
	}
}