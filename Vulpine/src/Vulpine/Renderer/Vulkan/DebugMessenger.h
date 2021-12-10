#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class DebugMessenger
	{
	public:
		static void Prepare();
		static void Create(VkInstance instance);
		static void Destroy(VkInstance instance);

		inline static VkDebugUtilsMessengerCreateInfoEXT* GetDebugMessengerInfo();

	private:
		static VkDebugUtilsMessengerEXT s_DebugMessenger;
		static VkDebugUtilsMessengerCreateInfoEXT s_DebugMessengerInfo;
	};
}

