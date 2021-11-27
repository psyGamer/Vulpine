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

	// Descriptor Set Layout
	VkDescriptorSetLayout DescriptorSetLayout::s_SetLayout = VK_NULL_HANDLE;

	void DescriptorSetLayout::Create()
	{
		VkDescriptorSetLayoutBinding setLayoutBinding{};
		setLayoutBinding.binding = 0;
		setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		setLayoutBinding.descriptorCount = 1;

		setLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
		setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setLayoutInfo.bindingCount = 1;
		setLayoutInfo.pBindings = &setLayoutBinding;

		VP_ASSERT_VK(vkCreateDescriptorSetLayout(Device::GetLogicalDevice(), &setLayoutInfo, nullptr, &s_SetLayout), "Failed to create descriptor set layout!");
	}

	void DescriptorSetLayout::Destroy()
	{
		vkDestroyDescriptorSetLayout(Device::GetLogicalDevice(), s_SetLayout, nullptr);
	}

	// Descriptor Set
	std::vector<VkDescriptorSet> DescriptorSet::s_Sets;

	void DescriptorSet::Create(const std::vector<VkDescriptorSetLayout>& layouts)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = DescriptorPool::GetPool();
		allocInfo.descriptorSetCount = static_cast<uint32_t>(Swapchain::GetImages().size());
		allocInfo.pSetLayouts = layouts.data();

		s_Sets.resize(Swapchain::GetImages().size());
		VP_ASSERT_VK(vkAllocateDescriptorSets(Device::GetLogicalDevice(), &allocInfo, s_Sets.data()), "Failed to allocate descriptor sets!");
	}
}
