#pragma once

#pragma warning(disable : 26812) // Vulkan issue

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Vulpine
{
	class Window
	{
	public:
		static void Create(std::string windowTitle, uint32_t width, uint32_t height);
		static void Destroy();

		static void CreateSurface(VkInstance instance);
		static void DestroySurface(VkInstance instance);

		static VkExtent2D QueryFramebufferSize();

		static void SetWindowTitle(std::string windowTitle);

		static GLFWwindow* const GetWindow() { return s_Window; }
		static std::string const GetWindowTitle() { return s_WindowTitle; }

		static uint32_t GetWidth() { return s_Width; }
		static uint32_t GetHeight() { return s_Height; }

		static VkSurfaceKHR GetSurface() { return s_Surface; }

	private:
		static void OnWindowResize(GLFWwindow* window, int width, int height);

	private:
		static GLFWwindow* s_Window;
		static std::string s_WindowTitle;

		static int s_Width, s_Height;

		static VkSurfaceKHR s_Surface;
	};
}

