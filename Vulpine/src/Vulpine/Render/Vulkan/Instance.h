#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Instance
	{
	public:
		static void Create();
		static void Destory();

		static const VkInstance& GetInstance() { return s_Instance; }
	private:
		Instance();

	private:
		static VkInstance s_Instance;
	};
}