#include "vppch.h"
#include "VertexBuffer.h"

namespace Vulpine::Vulkan
{
	VertexBuffer::VertexBuffer()
		: m_pBuffer(nullptr)
	{ }
	VertexBuffer::VertexBuffer(const std::initializer_list<DataType>& vertexAttributes, uint32_t vertexCount)
		: m_pBuffer(nullptr)
	{
		SetLayout(vertexAttributes, vertexCount);
	}

	void VertexBuffer::SetLayout(const std::initializer_list<DataType>& vertexAttributes, uint32_t vertexCount)
	{
		m_VertexAttributes = vertexAttributes;

		size_t size = 0;

		for (const auto& vertexAttribute : vertexAttributes)
			size += DataTypeHelper::QueryDataTypeSize(vertexAttribute);
		size *= vertexCount;

		m_pBuffer = std::make_shared<GpuBuffer>(size,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_VertexCount = vertexCount;
	}

	void VertexBuffer::SetData(const void* const data)
	{
		m_pBuffer->SetData(data);
	}

	VkVertexInputBindingDescription VertexBuffer::QueryBindingDescriptions(uint32_t bindingIndex) const
	{
		uint32_t stride = 0;

		for (const auto& vertexAttribute : m_VertexAttributes)
			stride += DataTypeHelper::QueryDataTypeSize(vertexAttribute);

		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = bindingIndex;
		bindingDescription.stride = stride;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	void VertexBuffer::QueryAttributeDescriptions(uint32_t bindingIndex, std::vector<VkVertexInputAttributeDescription>* attributeDescriptionsBuffer) const
	{
		uint32_t location = 0, offset = 0;

		attributeDescriptionsBuffer->reserve(m_VertexAttributes.size());

		for (const auto& vertexAttribute : m_VertexAttributes)
		{
			VkVertexInputAttributeDescription& attributeDescription = attributeDescriptionsBuffer->emplace_back();
			attributeDescription.binding = bindingIndex;
			attributeDescription.location = location++;
			attributeDescription.format = DataTypeHelper::QueryDataTypeFormat(vertexAttribute);
			attributeDescription.offset = offset;

			offset += DataTypeHelper::QueryDataTypeSize(vertexAttribute);
		}
	}
}
