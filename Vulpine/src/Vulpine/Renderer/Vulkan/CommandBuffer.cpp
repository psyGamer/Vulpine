#include "vppch.h"
#include "CommandBuffer.h"

#include "Device.h"
#include "RenderPass.h"
#include "Swapchain.h"

#include "Pipeline.h"

#include "Semaphore.h"

namespace Vulpine::Vulkan
{
	// CommandBuffers
	CommandBuffer::CommandBuffer(std::shared_ptr<const CommandPool> pCommandPool, uint32_t commandBufferCount)
		: m_pCommandPool(pCommandPool)
	{
		m_CommandBuffers.resize(commandBufferCount);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		allocInfo.commandPool = m_pCommandPool->GetPool();
		allocInfo.commandBufferCount = commandBufferCount;

		VP_ASSERT_VK(vkAllocateCommandBuffers(Device::GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()), "Failed to create command buffers!");
	}

	CommandBuffer::~CommandBuffer()
	{
		vkQueueWaitIdle(m_pCommandPool->GetQueue());
		vkFreeCommandBuffers(Device::GetLogicalDevice(), m_pCommandPool->GetPool(), m_CommandBuffers.size(), m_CommandBuffers.data());
	}

	void CommandBuffer::Submit(uint32_t commandBufferIndex, const Semaphore* pWaitSemaphore, const Semaphore* pSignalSemaphore)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[commandBufferIndex];

		if (pWaitSemaphore != nullptr)
		{
			VkSemaphore waitSemaphores[] = { pWaitSemaphore->GetSemaphore() };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
		}
		if (pSignalSemaphore != nullptr)
		{
			VkSemaphore signalSemaphores[] = { pSignalSemaphore->GetSemaphore() };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;
		}

		submitInfo.pWaitDstStageMask = waitStages;

		VP_ASSERT_VK(vkQueueSubmit(m_pCommandPool->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit draw command buffer!");
	}

	GraphicsCommandBuffer::GraphicsCommandBuffer()
		: CommandBuffer(GraphicsCommandPool::GetPool(), Swapchain::GetImages().size())
	{ }

	void GraphicsCommandBuffer::Record(const Pipeline& pipeline)
	{
		for (size_t i = 0; i < m_CommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			VP_ASSERT_VK(vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo), "Failed to begin recording command buffer!");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

			renderPassInfo.renderPass = RenderPass::GetRenderPass();
			renderPassInfo.framebuffer = Swapchain::GetFramebuffers()[i];

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = Swapchain::GetFramebufferSize();

			VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());

			vkCmdSetViewport(m_CommandBuffers[i], 0, 1, &pipeline.GetViewport());
			vkCmdSetScissor(m_CommandBuffers[i], 0, 1, &pipeline.GetScissor());

			const VkDeviceSize offsets[] = { 0 };
			const VkBuffer vertexBuffer = pipeline.GetVertexBuffer()->GetBuffer();

			vkCmdBindVertexBuffers(m_CommandBuffers[i], 0, 1, &vertexBuffer, offsets);
			vkCmdBindIndexBuffer(m_CommandBuffers[i], pipeline.GetIndexBuffer()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, &pipeline.GetUniformBuffer()->GetSets()[i], 0, nullptr);
#if 0
			vkCmdDraw(m_CommandBuffers[i], pipeline.GetVertexBuffer()->GetVertexCount(), 1, 0, 0);
#else
			vkCmdDrawIndexed(m_CommandBuffers[i], pipeline.GetIndexBuffer()->GetIndexCount(), 1, 0, 0, 0);
#endif
			vkCmdEndRenderPass(m_CommandBuffers[i]);

			VP_ASSERT_VK(vkEndCommandBuffer(m_CommandBuffers[i]), "Failed to record command buffer!");
		}
	}

	TransferCommandBuffer::TransferCommandBuffer()
		: CommandBuffer(TransferCommandPool::GetPool(), 1)
	{ }

	void TransferCommandBuffer::Record(VkBuffer souce, VkBuffer destination, const VkBufferCopy& bufferCopy)
	{
		for (size_t i = 0; i < m_CommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			VP_ASSERT_VK(vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo), "Failed to begin recording command buffer!");

			vkCmdCopyBuffer(m_CommandBuffers[i], souce, destination, 1, &bufferCopy);

			VP_ASSERT_VK(vkEndCommandBuffer(m_CommandBuffers[i]), "Failed to record command buffer!");
		}
	}

	void TransferCommandBuffer::Submit(const Semaphore* pWaitSemaphore, const Semaphore* pSignalSemaphore)
	{
		CommandBuffer::Submit(0, pWaitSemaphore, pSignalSemaphore);
	}
}