#include "vppch.h"
#include "Commands.h"

#include "Device.h"
#include "RenderPass.h"
#include "Swapchain.h"

#include "Pipeline.h"

#include "Semaphore.h"

namespace Vulpine::Vulkan
{
	VkCommandPool CommandPool::s_CommandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> CommandBuffers::s_CommandBuffers;

	// CommandPool
	void CommandPool::Create()
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		commandPoolInfo.queueFamilyIndex = Device::GetPhysicalDeviceInfo().GraphicsQueueIndex.value();
		commandPoolInfo.flags = 0;

		VP_ASSERT_VK(vkCreateCommandPool(Device::GetLogicalDevice(), &commandPoolInfo, nullptr, &s_CommandPool), "Failed to create command pool!");
	}

	void CommandPool::Destroy()
	{
		vkDestroyCommandPool(Device::GetLogicalDevice(), s_CommandPool, nullptr);
	}

	// CommandBuffers
	void CommandBuffers::Create()
	{
		s_CommandBuffers.resize(Swapchain::GetFramebuffers().size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = CommandPool::GetCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(s_CommandBuffers.size());

		VP_ASSERT_VK(vkAllocateCommandBuffers(Device::GetLogicalDevice(), &allocInfo, s_CommandBuffers.data()), "Failed to create command buffers!");
	}

	void CommandBuffers::Record(const Pipeline& pipeline)
	{
		for (size_t i = 0; i < s_CommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr;

			VP_ASSERT_VK(vkBeginCommandBuffer(s_CommandBuffers[i], &beginInfo), "Failed to begin recording command buffer!");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

			renderPassInfo.renderPass = RenderPass::GetRenderPass();
			renderPassInfo.framebuffer = Swapchain::GetFramebuffers()[i];

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = Swapchain::GetFramebufferSize();

			VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(s_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(s_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());

			vkCmdSetViewport(s_CommandBuffers[i], 0, 1, &pipeline.GetViewport());
			vkCmdSetScissor(s_CommandBuffers[i], 0, 1, &pipeline.GetScissor());

			const VkDeviceSize offsets[] = { 0 };
			const VkBuffer vertexBuffer = pipeline.GetVertexBuffer()->GetBuffer();

			vkCmdBindVertexBuffers(s_CommandBuffers[i], 0, 1, &vertexBuffer, offsets);

			vkCmdDraw(s_CommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(s_CommandBuffers[i]);

			VP_ASSERT_VK(vkEndCommandBuffer(s_CommandBuffers[i]), "Failed to record command buffer!");
		}
	}

	void CommandBuffers::Destroy()
	{
		vkFreeCommandBuffers(Device::GetLogicalDevice(), CommandPool::GetCommandPool(), s_CommandBuffers.size(), s_CommandBuffers.data());
	}

	void CommandBuffers::Submit(uint32_t commandBufferIndex, const Semaphore& waitSemaphore, const Semaphore& signalSemaphore)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { waitSemaphore.GetSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &s_CommandBuffers[commandBufferIndex];

		VkSemaphore signalSemaphores[] = { signalSemaphore.GetSemaphore() };

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VP_ASSERT_VK(vkQueueSubmit(Device::GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit draw command buffer!");
	}
}