#pragma once

#include "Buffer.h"

namespace Vulpine::Vulkan
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		IndexBuffer(uint32_t vertexCount);

		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer(const IndexBuffer& other) noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(const IndexBuffer& other) noexcept;

		void SetData(const void* const data);
		void SetIndexCount(uint32_t indexCount);

		inline VkBuffer GetBuffer() const { return m_pBuffer->GetBuffer(); }
		inline uint32_t GetIndexCount() const { return m_IndexCount; }

	private:
		std::shared_ptr<Buffer> m_pBuffer;

		uint32_t m_IndexCount;
	};
}

