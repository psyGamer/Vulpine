#include "vppch.h"
#include "RenderPass.h"

#include "Device.h"
#include "Swapchain.h"

namespace Vulpine::Vulkan
{
	VkRenderPass RenderPass::s_RenderPass = VK_NULL_HANDLE;

	void RenderPass::Create()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = Swapchain::s_ImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentReference{};
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentReference;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		VP_ASSERT_VK(vkCreateRenderPass(Device::GetLogicalDevice(), &renderPassInfo, nullptr, &s_RenderPass), "Failed to create render pass!");
	}

	void RenderPass::Destroy()
	{
		vkDestroyRenderPass(Device::GetLogicalDevice(), s_RenderPass, nullptr);
	}
}
