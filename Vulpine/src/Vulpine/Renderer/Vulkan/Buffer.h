#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Buffer
	{
	public:
		Buffer(size_t bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);
		~Buffer();

		void SetData(const void* const data);

		static void Copy(const Buffer& source, const Buffer& target);

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
}

