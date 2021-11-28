#include "vppch.h"
#include "Instance.h"

#include "Core/Window.h"

#include "DebugMessenger.h"
#include "Device.h"

namespace Vulpine::Vulkan
{
	VkInstance Instance::s_Instance = VK_NULL_HANDLE;

	void Instance::Create()
	{
		VP_ASSERT(Window::GetWindow() == nullptr, "Window not yet initialized!");

		// Application Info
	
		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		
		applicationInfo.pApplicationName = Window::GetWindowTitle().c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Vulpine Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		// Instance Info

		DebugMessenger::Prepare();

		const std::vector<const char*> instanceLayers = {
#ifdef VP_VULKAN_VALIDATION
		"VK_LAYER_KHRONOS_validation"
#endif
		};

		const std::vector<const char*> instanceExtensions = QueryRequiredExtensions();

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pNext = DebugMessenger::GetDebugMessengerInfo();

		instanceInfo.pApplicationInfo = &applicationInfo;

		instanceInfo.enabledLayerCount = instanceLayers.size();
		instanceInfo.ppEnabledLayerNames = instanceLayers.data();

		instanceInfo.enabledExtensionCount = instanceExtensions.size();
		instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();

		VP_ASSERT_VK(vkCreateInstance(&instanceInfo, nullptr, &s_Instance), "Failed to create instance");

		DebugMessenger::Create(s_Instance);
		Window::CreateSurface(s_Instance);
		Device::Create(s_Instance);
	}

	void Instance::Destory()
	{
		Device::Destory();
		Window::DestroySurface(s_Instance);
		DebugMessenger::Destroy(s_Instance);

		vkDestroyInstance(s_Instance, nullptr);
	}

	std::vector<const char*> Instance::QueryRequiredExtensions()
	{
		uint32_t glfwExtensionCount;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef VP_VULKAN_VALIDATION
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		return extensions;
	}
}