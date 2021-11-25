#include "vppch.h"
#include "VertexBuffer.h"

namespace Vulpine::Vulkan
{
	VertexBuffer::VertexBuffer()
		: m_pBuffer(nullptr)
	{ }
	VertexBuffer::VertexBuffer(const std::initializer_list<VertexAttribute>& vertexAttributes, uint32_t vertexCount)
		: m_pBuffer(nullptr)
	{
		SetLayout(vertexAttributes, vertexCount);
	}

	void VertexBuffer::SetLayout(const std::initializer_list<VertexAttribute>& vertexAttributes, uint32_t vertexCount)
	{
		m_VertexAttributes = vertexAttributes;

		size_t size = 0;

		for (const auto& vertexAttribute : vertexAttributes)
			size += QueryVertexAttributeSize(vertexAttribute);
		size *= vertexCount;

		m_pBuffer = CreateReference<Buffer>(size,
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
			stride += QueryVertexAttributeSize(vertexAttribute);

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
			attributeDescription.format = QueryVertexAttributeFormat(vertexAttribute);
			attributeDescription.offset = offset;

			offset += QueryVertexAttributeSize(vertexAttribute);
		}
	}

	uint32_t VertexBuffer::QueryVertexAttributeSize(const VertexAttribute& vertexAttribute)
	{
		switch (vertexAttribute)
		{
		case VertexAttribute::INT8:
		case VertexAttribute::UINT8:
			return 1;
			
		case VertexAttribute::INT16:
		case VertexAttribute::UINT16:
			return 2;

		case VertexAttribute::INT32:
		case VertexAttribute::UINT32:
		case VertexAttribute::FLOAT32:
			return 4;

		case VertexAttribute::FLOAT64:
			return 8;

		case VertexAttribute::I32VEC2:
		case VertexAttribute::F32VEC2:
		case VertexAttribute::U32VEC2:
			return 4 * 2;
		case VertexAttribute::I32VEC3:
		case VertexAttribute::F32VEC3:
		case VertexAttribute::U32VEC3:
			return 4 * 3;
		case VertexAttribute::U32VEC4:
		case VertexAttribute::F32VEC4:
		case VertexAttribute::I32VEC4:
			return 4 * 4;

		case VertexAttribute::F64VEC2:
			return 8 * 2;
		case VertexAttribute::F64VEC3:
			return 8 * 3;
		case VertexAttribute::F64VEC4:
			return 8 * 4;
		}
	}

	VkFormat VertexBuffer::QueryVertexAttributeFormat(const VertexAttribute& vertexAttribute)
	{
		switch (vertexAttribute)
		{
		case VertexAttribute::INT8:
			return VK_FORMAT_R8_SINT;
		case VertexAttribute::UINT8:
			return VK_FORMAT_R8_UINT;

		case VertexAttribute::INT16:
			return VK_FORMAT_R16_SINT;
		case VertexAttribute::UINT16:
			return VK_FORMAT_R16_UINT;

		case VertexAttribute::INT32:
			return VK_FORMAT_R32_SINT;
		case VertexAttribute::UINT32:
			return VK_FORMAT_R32_UINT;

		case VertexAttribute::FLOAT32:
			return VK_FORMAT_R32_SFLOAT;
		case VertexAttribute::FLOAT64:
			return VK_FORMAT_R64_SFLOAT;

		case VertexAttribute::I32VEC2:
			return VK_FORMAT_R32G32_SINT;
		case VertexAttribute::U32VEC2:
			return VK_FORMAT_R32G32_UINT;
		case VertexAttribute::F32VEC2:
			return VK_FORMAT_R32G32_SFLOAT;

		case VertexAttribute::I32VEC3:
			return VK_FORMAT_R32G32B32_SINT;
		case VertexAttribute::U32VEC3:
			return VK_FORMAT_R32G32B32_UINT;
		case VertexAttribute::F32VEC3:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case VertexAttribute::I32VEC4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case VertexAttribute::U32VEC4:
			return VK_FORMAT_R32G32B32A32_UINT;
		case VertexAttribute::F32VEC4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case VertexAttribute::F64VEC2:
			return VK_FORMAT_R64G64_SFLOAT;
		case VertexAttribute::F64VEC3:
			return VK_FORMAT_R64G64B64_SFLOAT;
		case VertexAttribute::F64VEC4:
			return VK_FORMAT_R64G64B64A64_SFLOAT;
		}
	}
}
