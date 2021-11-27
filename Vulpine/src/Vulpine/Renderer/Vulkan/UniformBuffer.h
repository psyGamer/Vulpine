#pragma once

#include "Vulkan.h"

#include "Buffer.h"

namespace Vulpine::Vulkan
{
	class DescriptorPool
	{
	public:
		static void Create();
		static void Destroy();

		static VkDescriptorPool GetPool() { return s_Pool; }

	private:
		static VkDescriptorPool s_Pool;
	};

	class DescriptorSetLayout
	{
	public:
		static void Create();
		static void Destroy();

		static VkDescriptorSetLayout GetSetLayout() { return s_SetLayout; }

	private:
		static VkDescriptorSetLayout s_SetLayout;
	};
}
