#pragma once

#include <GLFW/glfw3.h>

namespace Vulpine
{
	class Window
	{
	public:
		static void Create(const std::string& windowTitle, uint32_t width, uint32_t height);
		static void Destroy();

		GLFWwindow* const GetWindow() { return s_Window; }

		uint32_t GetWidth() { return s_Width; }
		uint32_t GetHeight() { return s_Height; }

	private:
		static void OnWindowResize(GLFWwindow* window, int width, int height);

	private:
		static GLFWwindow* s_Window;

		static uint32_t s_Width, s_Height;
	};
}

