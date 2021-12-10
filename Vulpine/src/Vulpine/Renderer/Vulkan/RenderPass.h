#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Swapchain;

	class RenderPass
	{
	public:
		static void Create();
		static void Destroy();

		inline static VkRenderPass GetRenderPass() { return s_RenderPass; }

	private:
		RenderPass();

	private:
		static VkRenderPass s_RenderPass;
	};
}

