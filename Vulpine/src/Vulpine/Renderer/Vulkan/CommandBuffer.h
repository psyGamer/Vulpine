#pragma once

#include "Vulkan.h"

#include "CommandPool.h"

namespace Vulpine::Vulkan
{
	class Pipeline;
	class Semaphore;

	class CommandBuffer
	{
	public:
		CommandBuffer(std::shared_ptr<const CommandPool> pCommandPool, uint32_t commandBufferCount);
		~CommandBuffer();
		
		void Submit(uint32_t commandBufferIndex, const Semaphore* pWaitSemaphore = nullptr, const Semaphore* pSignalSemaphore = nullptr);

		inline const std::vector<VkCommandBuffer>& GetCommandBuffers() const { return m_CommandBuffers; }

	protected:
		std::vector<VkCommandBuffer> m_CommandBuffers;

	private:
		std::shared_ptr<const CommandPool> m_pCommandPool;
	};

	class GraphicsCommandBuffer : public CommandBuffer
	{
	public:
		GraphicsCommandBuffer();

		void Record(const Pipeline& pipeline);
	};

	class TransferCommandBuffer : public CommandBuffer
	{
	public:
		TransferCommandBuffer();

		void Record(VkBuffer souce, VkBuffer destination, const VkBufferCopy& bufferCopy);
		void Submit(const Semaphore* pWaitSemaphore = nullptr, const Semaphore* pSignalSemaphore = nullptr);
	};
};

