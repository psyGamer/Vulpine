#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Buffer
	{
	public:
		Buffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);
		~Buffer();

		virtual void SetData(const void* const data) = 0;

		static void Copy(const Buffer& source, const Buffer& destination);

		inline void CopyFrom(const Buffer& source) { Copy(source, *this); }
		inline void CopyTo(const Buffer& target) { Copy(*this, target); }

		VkBuffer GetBuffer() const { return m_Buffer; }
		VkDeviceMemory GetBufferMemory() const { return m_BufferMemory; }
	private:
		static uint32_t FindMemoryTypeIndex(uint32_t supportedTypesBitmask, VkMemoryPropertyFlags requiredTypes);

	public:
		VkDeviceSize m_BufferSize;

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
	};

	class CpuBuffer : public Buffer
	{
	public:
		CpuBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags);
		CpuBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);

		void SetData(const void* const data) override;
	};

	class GpuBuffer : public Buffer
	{
	public:
		GpuBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags);
		GpuBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);

		void SetData(const void* const data) override;
	};
}
