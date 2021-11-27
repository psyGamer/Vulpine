#include "vppch.h"
#include "UniformBuffer.h"

#include "Device.h"
#include "Swapchain.h"

namespace Vulpine::Vulkan
{
	// Descriptor Pool
	VkDescriptorPool DescriptorPool::s_Pool = VK_NULL_HANDLE;

	void DescriptorPool::Create()
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(Swapchain::GetImages().size());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;

		poolInfo.maxSets = static_cast<uint32_t>(Swapchain::GetImages().size());

		VP_ASSERT_VK(vkCreateDescriptorPool(Device::GetLogicalDevice(), &poolInfo, nullptr, &s_Pool), "Failed to create descriptor pool!");
	}

	void DescriptorPool::Destroy()
	{
		vkDestroyDescriptorPool(Device::GetLogicalDevice(), s_Pool, nullptr);
	}
}
