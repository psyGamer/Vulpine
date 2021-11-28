#include "vppch.h"
#include "CommandPool.h"

#include "Device.h"

namespace Vulpine::Vulkan
{
	CommandPool::CommandPool(VkQueue queue, VkCommandPoolCreateFlags flags)
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		commandPoolInfo.queueFamilyIndex = queue;
		commandPoolInfo.flags = flags;

		VP_ASSERT_VK(vkCreateCommandPool(Device::GetLogicalDevice(), &commandPoolInfo, nullptr, &m_CommandPool), "Failed to create command pool!");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(Device::GetLogicalDevice(), m_CommandPool, nullptr);
	}

	Reference<CommandPool> GraphicsCommandPool::s_pCommandPool = nullptr;
	Reference<CommandPool> TransferCommandPool::s_pCommandPool = nullptr;
}