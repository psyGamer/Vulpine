#include "vppch.h"
#include "Window.h"

namespace Vulpine
{
	void Window::Create(const std::string& windowTitle, uint32_t width, uint32_t height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		s_Window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);

		glfwSetWindowSizeCallback(s_Window, OnWindowResize);
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(s_Window);
		glfwTerminate();
	}

	void Window::OnWindowResize(GLFWwindow* window, int width, int height)
	{

	}
}