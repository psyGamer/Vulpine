#include "vppch.h"
#include "DebugMessenger.h"

namespace Vulpine::Vulkan
{
#ifdef VP_VULKAN_VALIDATION
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
#endif

	VkDebugUtilsMessengerEXT DebugMessenger::s_DebugMessenger = VK_NULL_HANDLE;
	VkDebugUtilsMessengerCreateInfoEXT DebugMessenger::s_DebugMessengerInfo = {};

	void DebugMessenger::Prepare()
	{
#ifdef VP_VULKAN_VALIDATION
		s_DebugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

#if VP_DEBUG
		s_DebugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
#else
		s_DebugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
#endif
		s_DebugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		s_DebugMessengerInfo.pfnUserCallback = debugCallback;
#endif
	}

	void DebugMessenger::Create(VkInstance instance)
	{
#ifdef VP_VULKAN_VALIDATION
		auto createDebugUtilsMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (createDebugUtilsMessengerFunc != nullptr)
		{
			VP_ASSERT_VK(createDebugUtilsMessengerFunc(instance, &s_DebugMessengerInfo, nullptr, &s_DebugMessenger), "Failed to create debug messenger!");
		}
		else
		{
			VP_ERROR("Debug messenger extensions is not present!");
		}
#endif
	}

	void DebugMessenger::Destroy(VkInstance instance)
	{
#ifdef VP_VULKAN_VALIDATION
		auto destroyDebugUtilsMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		if (destroyDebugUtilsMessengerFunc != nullptr)
		{
			destroyDebugUtilsMessengerFunc(instance, s_DebugMessenger, nullptr);
		}
#endif
	}

	VkDebugUtilsMessengerCreateInfoEXT* DebugMessenger::GetDebugMessengerInfo()
	{
#ifdef VP_VULKAN_VALIDATION
		return &s_DebugMessengerInfo;
#else
		return nullptr;
#endif
	}
}
