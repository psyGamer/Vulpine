#include "vppch.h"
#include "Instance.h"

#include "Core/Window.h"

#include "Device.h"

#ifdef VP_DEBUG
#define VULKAN_VALIDATION
#endif

namespace Vulpine::Vulkan
{
	VkInstance Instance::s_Instance = VK_NULL_HANDLE;

#ifdef VULKAN_VALIDATION
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::string message;

		if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
			message = "(Performance) " + std::string(pCallbackData->pMessage);
		else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
			message = std::string(pCallbackData->pMessage);
		else
			message = "(General) " + std::string(pCallbackData->pMessage);

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			VP_ERROR(message);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			VP_WARN(message);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			VP_INFO(message);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			VP_TRACE(message);

		return VK_FALSE;
	}

	static VkDebugUtilsMessengerEXT debugMessenger;
#endif

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

		const std::vector<const char*> instanceLayers = {
#ifdef VULKAN_VALIDATION
		"VK_LAYER_KHRONOS_validation"
#endif
		};

		uint32_t glfwExtensionCount;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#ifdef VULKAN_VALIDATION
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo{};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = debugCallback;
#endif

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

#ifdef VULKAN_VALIDATION
		instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugMessengerInfo;
#endif

		instanceInfo.pApplicationInfo = &applicationInfo;

		instanceInfo.enabledLayerCount = instanceLayers.size();
		instanceInfo.ppEnabledLayerNames = instanceLayers.data();
#ifdef VULKAN_VALIDATION
		instanceInfo.enabledExtensionCount = extensions.size();
		instanceInfo.ppEnabledExtensionNames = extensions.data();
#else
		instanceInfo.enabledExtensionCount = glfwExtensionCount;
		instanceInfo.ppEnabledExtensionNames = glfwExtensions;
#endif

		VP_ASSERT_VK(vkCreateInstance(&instanceInfo, nullptr, &s_Instance), "Failed to create instance");

#ifdef VULKAN_VALIDATION
		auto createDebugUtilsMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_Instance, "vkCreateDebugUtilsMessengerEXT");

		if (createDebugUtilsMessengerFunc != nullptr)
		{
			VP_ASSERT_VK(createDebugUtilsMessengerFunc(s_Instance, &debugMessengerInfo, nullptr, &debugMessenger), "Failed to create debug messenger!");
		}
		else
		{
			VP_ERROR("Debug messenger extensions is not present!");
		}
#endif
		Window::CreateSurface(s_Instance);

		Device::Create(s_Instance);
	}

	void Instance::Destory()
	{
		Device::Destory();

		Window::DestroySurface(s_Instance);

#ifdef VULKAN_VALIDATION
		auto destroyDebugUtilsMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (destroyDebugUtilsMessengerFunc != nullptr)
		{
			destroyDebugUtilsMessengerFunc(s_Instance, debugMessenger, nullptr);
		}
#endif

		vkDestroyInstance(s_Instance, nullptr);
	}
}