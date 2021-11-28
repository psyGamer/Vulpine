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

	class CommandBuffer
	{
	public:
		CommandBuffer(uint32_t commandBufferCount);
		~CommandBuffer();
		
		void Submit(uint32_t commandBufferIndex, const Semaphore& waitSemaphore, const Semaphore& signalSemaphore);

		const std::vector<VkCommandBuffer>& GetCommandBuffers() const { return m_CommandBuffers; }

	private:
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
};

