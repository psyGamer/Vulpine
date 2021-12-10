#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Instance
	{
	public:
		static void Create();
		static void Destory();

		inline static VkInstance GetInstance() { return s_Instance; }

	private:
		Instance();

		static std::vector<const char*> QueryRequiredExtensions();

	private:
		static VkInstance s_Instance;
	};
}