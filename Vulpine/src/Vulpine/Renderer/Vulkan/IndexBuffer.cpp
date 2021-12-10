#include "vppch.h"
#include "IndexBuffer.h"

namespace Vulpine::Vulkan
{
	IndexBuffer::IndexBuffer()
		: m_pBuffer(nullptr) { }
	IndexBuffer::IndexBuffer(uint32_t vertexCount)
	{
		SetIndexCount(vertexCount);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other)
		: m_pBuffer(std::exchange(other.m_pBuffer, nullptr)),
		m_IndexCount(std::move(other.m_IndexCount))
	{}

	IndexBuffer::IndexBuffer(const IndexBuffer& other)
		: m_pBuffer(other.m_pBuffer),
		m_IndexCount(other.m_IndexCount)
	{}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		this->~IndexBuffer();

		m_pBuffer = std::exchange(other.m_pBuffer, nullptr);
		m_IndexCount = std::move(other.m_IndexCount);
	}

	IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other) noexcept
	{
		this->~IndexBuffer();

		m_pBuffer = other.m_pBuffer;
		m_IndexCount = other.m_IndexCount;
	}

	void IndexBuffer::SetData(const void* const data)
	{
		m_pBuffer->SetData(data);
	}

	void IndexBuffer::SetIndexCount(uint32_t indexCount)
	{
		m_pBuffer = std::make_shared<GpuBuffer>(sizeof(uint32_t) * indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_IndexCount = indexCount;
	}
}
