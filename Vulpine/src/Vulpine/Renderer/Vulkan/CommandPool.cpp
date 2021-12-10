#include "vppch.h"
#include "CommandPool.h"

#include "Device.h"

namespace Vulpine::Vulkan
{
	CommandPool::CommandPool(VkQueue queue, uint32_t queueIndex, VkCommandPoolCreateFlags flags)
		: m_Queue(queue), m_CommandPool(VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		commandPoolInfo.queueFamilyIndex = queueIndex;
		commandPoolInfo.flags = flags;

		VP_ASSERT_VK(vkCreateCommandPool(Device::GetLogicalDevice(), &commandPoolInfo, nullptr, &m_CommandPool), "Failed to create command pool!");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(Device::GetLogicalDevice(), m_CommandPool, nullptr);
	}

	CommandPool::CommandPool(CommandPool&& other)
		: m_CommandPool(std::exchange(other.m_CommandPool, VK_NULL_HANDLE)),
		m_Queue(std::exchange(other.m_Queue, VK_NULL_HANDLE))
	{ }

	CommandPool::CommandPool(const CommandPool& other)
		: m_CommandPool(other.m_CommandPool),
		m_Queue(other.m_Queue)
	{ }

	CommandPool& CommandPool::operator=(CommandPool&& other) noexcept
	{
		this->~CommandPool();

		m_CommandPool = std::exchange(other.m_CommandPool, VK_NULL_HANDLE);
		m_Queue = std::exchange(other.m_Queue, VK_NULL_HANDLE);
	}

	CommandPool& CommandPool::operator=(const CommandPool& other) noexcept
	{
		this->~CommandPool();

		m_CommandPool = other.m_CommandPool;
		m_Queue = other.m_Queue;
	}

	std::shared_ptr<CommandPool> GraphicsCommandPool::s_pCommandPool = nullptr;
	std::shared_ptr<CommandPool> TransferCommandPool::s_pCommandPool = nullptr;

	void GraphicsCommandPool::Create()
	{
		s_pCommandPool = std::make_shared<CommandPool>(
			Device::GetGraphicsQueue(), Device::GetPhysicalDeviceInfo().GraphicsQueueIndex.value()
		);
	}

	void TransferCommandPool::Create()
	{
		s_pCommandPool = std::make_shared<CommandPool>(
			Device::GetTransferQueue(), Device::GetPhysicalDeviceInfo().TransferQueueIndex.value()
		);
	}
}
