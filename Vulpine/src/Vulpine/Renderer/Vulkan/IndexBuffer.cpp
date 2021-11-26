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

	void IndexBuffer::SetData(const void* const data)
	{
		m_pBuffer->SetData(data);
	}

	void IndexBuffer::SetIndexCount(uint32_t indexCount)
	{
		m_pBuffer = CreateReference<GpuBuffer>(sizeof(uint32_t) * indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_IndexCount = indexCount;
	}
}
