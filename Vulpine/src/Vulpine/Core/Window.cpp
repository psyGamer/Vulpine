#include "vppch.h"
#include "Window.h"

namespace Vulpine
{
	GLFWwindow* Window::s_Window = nullptr;
	std::string Window::s_WindowTitle = "";

	int Window::s_Width = 0, Window::s_Height = 0;

	VkSurfaceKHR Window::s_Surface = VK_NULL_HANDLE;
	
	void Window::Create(const std::string& windowTitle, uint32_t width, uint32_t height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		s_Window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
		s_WindowTitle = windowTitle;

		glfwSetWindowSizeCallback(s_Window, OnWindowResize);

		glfwGetWindowSize(s_Window, &s_Width, &s_Height);
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(s_Window);
		glfwTerminate();
	}

	void Window::CreateSurface(VkInstance instance)
	{
		glfwCreateWindowSurface(instance, s_Window, nullptr, &s_Surface);
	}

	void Window::DestroySurface(VkInstance instance)
	{
		vkDestroySurfaceKHR(instance, s_Surface, nullptr);
	}

	VkExtent2D Window::QueryFramebufferSize()
	{
		int width, height;
		glfwGetFramebufferSize(s_Window, &width, &height);

		return VkExtent2D{ 
			static_cast<uint32_t>(width), 
			static_cast<uint32_t>(height)
		};
	}

	void Window::SetWindowTitle(const std::string& windowTitle)
	{
		glfwSetWindowTitle(s_Window, windowTitle.c_str());
		s_WindowTitle = windowTitle;
	}

	void Window::OnWindowResize(GLFWwindow* window, int width, int height)
	{
		glfwGetWindowSize(s_Window, &s_Width, &s_Height);
	}
}