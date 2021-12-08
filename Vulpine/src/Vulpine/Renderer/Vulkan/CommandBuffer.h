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
		CommandBuffer(const CommandPool commandPool, uint32_t commandBufferCount);
		~CommandBuffer();
		
		void Submit(uint32_t commandBufferIndex, VkQueue queue, const Semaphore* pWaitSemaphore = nullptr, const Semaphore* pSignalSemaphore = nullptr);

		const std::vector<VkCommandBuffer>& GetCommandBuffers() const { return m_CommandBuffers; }

	protected:
		std::vector<VkCommandBuffer> m_CommandBuffers;
	private:
		CommandPool m_CommandPool;
	};

	class GraphicsCommandBuffer : public CommandBuffer
	{
	public:
		GraphicsCommandBuffer();

		void Record(const Pipeline& pipeline);
		void Submit(uint32_t commandBufferIndex, const Semaphore* pWaitSemaphore = nullptr, const Semaphore* pSignalSemaphore = nullptr);
	};

	class TransferCommandBuffer : public CommandBuffer
	{
	public:
		TransferCommandBuffer();

		void Record(VkBuffer souce, VkBuffer destination, const VkBufferCopy& bufferCopy);
		void Submit(const Semaphore* pWaitSemaphore = nullptr, const Semaphore* pSignalSemaphore = nullptr);
	};
};

