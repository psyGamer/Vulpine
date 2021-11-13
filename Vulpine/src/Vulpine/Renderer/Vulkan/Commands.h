#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class CommandPool
	{
	public:
		static void Create();
		static void Destroy();

		static const VkCommandPool& GetCommandPool() { return s_CommandPool; }

	private:
		static VkCommandPool s_CommandPool;
	};

	class Pipeline;
	class Semaphore;

	class CommandBuffers
	{
	public:
		static void Create();
		static void Record(const Pipeline& pipeline);
		static void Destroy();

		static void Submit(uint32_t commandBufferIndex, const Semaphore& waitSemaphore, const Semaphore& signalSemaphore);

		static const std::vector<VkCommandBuffer>& GetCommandBuffers() { return s_CommandBuffers; }

	private:
		static std::vector<VkCommandBuffer> s_CommandBuffers;
	};
};

