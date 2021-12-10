#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Buffer
	{
	public:
		Buffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);
		~Buffer();

		Buffer(Buffer&& other) noexcept;
		Buffer(const Buffer& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;
		Buffer& operator=(const Buffer& other) noexcept;

		virtual void SetData(const void* const data) = 0;

		static void Copy(const Buffer& source, const Buffer& destination);

		inline void CopyFrom(const Buffer& source) { Copy(source, *this); }
		inline void CopyTo(const Buffer& target) { Copy(*this, target); }

		inline VkBuffer GetBuffer() const { return m_Buffer; }
		inline VkDeviceMemory GetBufferMemory() const { return m_BufferMemory; }

	private:
		static uint32_t FindMemoryTypeIndex(uint32_t supportedTypesBitmask, VkMemoryPropertyFlags requiredTypes);

	protected:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

		VkDeviceSize m_BufferSize;

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
