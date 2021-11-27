#include "vppch.h"
#include "Buffer.h"

#include "Device.h"
#include "Commands.h"

namespace Vulpine::Vulkan
{
	Buffer::Buffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
	{
		m_BufferSize = bufferSize;

		// Creation
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;

		bufferInfo.usage = usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		bufferInfo.queueFamilyIndexCount = 0;
		bufferInfo.pQueueFamilyIndices = nullptr;

		VP_ASSERT_VK(vkCreateBuffer(Device::GetLogicalDevice(), &bufferInfo, nullptr, &m_Buffer), "Failed to create buffer!");

		// Allocation
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(Device::GetLogicalDevice(), m_Buffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo{};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryFlags);
		
		VP_ASSERT_VK(vkAllocateMemory(Device::GetLogicalDevice(), &memoryAllocateInfo, nullptr, &m_BufferMemory));
		VP_ASSERT_VK(vkBindBufferMemory(Device::GetLogicalDevice(), m_Buffer, m_BufferMemory, 0));
	}

	Buffer::~Buffer()
	{
		vkDestroyBuffer(Device::GetLogicalDevice(), m_Buffer, nullptr);
		vkFreeMemory(Device::GetLogicalDevice(), m_BufferMemory, nullptr);
	}

	void Buffer::Copy(const Buffer& source, const Buffer& destination)
	{
		// Start single time command buffer
		// TODO refactor
		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = CommandPool::GetCommandPool();
		commandBufferAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		VP_ASSERT_VK(vkAllocateCommandBuffers(Device::GetLogicalDevice(), &commandBufferAllocateInfo, &commandBuffer));

		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VP_ASSERT_VK(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

		// Copy buffer
		VkBufferCopy bufferCopy;
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = std::fmin(source.m_BufferSize, destination.m_BufferSize);

		vkCmdCopyBuffer(commandBuffer, source.m_Buffer, destination.m_Buffer, 1, &bufferCopy);

		// End single time command buffer
		// TODO refactor
		VP_ASSERT_VK(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VP_ASSERT_VK(vkQueueSubmit(Device::GetTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE));

		vkQueueWaitIdle(Device::GetTransferQueue());
		vkFreeCommandBuffers(Device::GetLogicalDevice(), CommandPool::GetCommandPool(), 1, &commandBuffer);
	}

	uint32_t Buffer::FindMemoryTypeIndex(uint32_t supportedTypesBitmask, VkMemoryPropertyFlags requiredTypes)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(Device::GetPhysicalDevice(), &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if ((supportedTypesBitmask & (1 << i)) && // Check that the memory type is supported
				(memoryProperties.memoryTypes[i].propertyFlags & requiredTypes) == requiredTypes) // Check that all required flags are supported
			{
				return i;
			}
		}

		VP_ERROR("Failed to find suitable memory type!");
	}

	// CPU Buffer
	CpuBuffer::CpuBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
		: Buffer(bufferSize, usageFlags, memoryFlags)
	{
		VP_ASSERT(
			(memoryFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0 ||
			(memoryFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) != 0 ||
			(memoryFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) != 0,

			"CPU buffer should not be located on the GPU!"
		);

		VP_ASSERT(!(
			(memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 ||
			(memoryFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0 ||
			(memoryFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0),

			"CPU buffer should be located on the CPU!"
		);
	}

	void CpuBuffer::SetData(const void* const data)
	{
		void* dataPtr;

		vkMapMemory(Device::GetLogicalDevice(), m_BufferMemory, 0, m_BufferSize, 0, &dataPtr);
		memcpy(dataPtr, data, static_cast<size_t>(m_BufferSize));
		vkUnmapMemory(Device::GetLogicalDevice(), m_BufferMemory);
	}

	// GPU Buffer
	GpuBuffer::GpuBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
		: Buffer(bufferSize, usageFlags, memoryFlags)
	{
		VP_ASSERT(
			(memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == memoryFlags ||
			(memoryFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == memoryFlags ||
			(memoryFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) == memoryFlags,

			"GPU buffer should not be located on the CPU!"
		);

		VP_ASSERT(!(
			(memoryFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == memoryFlags ||
			(memoryFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) == memoryFlags ||
			(memoryFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) == memoryFlags),

			"GPU buffer should be located on the GPU!"
		);
	}

	void GpuBuffer::SetData(const void* const data)
	{
		CpuBuffer stagingBuffer(m_BufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		stagingBuffer.SetData(data);

		CopyFrom(stagingBuffer);
	}
}