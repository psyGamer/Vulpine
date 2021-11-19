#include "vppch.h"
#include "VertexBuffer.h"

namespace Vulpine::Vulkan
{
	VertexBuffer::VertexBuffer()
	{ }
	VertexBuffer::VertexBuffer(const std::initializer_list<VertexAttribute>& vertexAttributes)
		: m_VertexAttributes(vertexAttributes)
	{ }

	void VertexBuffer::SetLayout(const std::initializer_list<VertexAttribute>& vertexAttributes)
	{
		m_VertexAttributes = vertexAttributes;
	}

	void VertexBuffer::SetData(const void* const data, uint32_t vertexCount)
	{
		size_t size = 0;

		for (const auto& vertexAttribute : m_VertexAttributes)
			size += QueryVertexAttributeSize(vertexAttribute);

		m_Data = data;
		m_DataSize = size * vertexCount;
	}

	VkVertexInputBindingDescription VertexBuffer::QueryBindingDescriptions(uint32_t bindingIndex)
	{
		uint32_t stride = 0;

		for (const auto& vertexAttribute : m_VertexAttributes)
			stride += QueryVertexAttributeSize(vertexAttribute);

		VkVertexInputBindingDescription bindingDescription;
		bindingDescription.binding = bindingIndex;
		bindingDescription.stride = stride;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	void VertexBuffer::QueryAttributeDescriptions(uint32_t bindingIndex, std::vector<VkVertexInputAttributeDescription>* attributeDescriptionBuffer)
	{
		uint32_t location = 0, offset = 0;

		attributeDescriptionBuffer->reserve(m_VertexAttributes.size());

		for (const auto& vertexAttribute : m_VertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription = attributeDescriptionBuffer->emplace_back();
			attributeDescription.location = location++;
			attributeDescription.binding = bindingIndex;
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
			return 8;
			
		case VertexAttribute::INT16:
		case VertexAttribute::UINT16:
			return 16;

		case VertexAttribute::INT32:
		case VertexAttribute::UINT32:
		case VertexAttribute::FLOAT32:
			return 32;

		case VertexAttribute::FLOAT64:
			return 64;

		case VertexAttribute::I32VEC2:
		case VertexAttribute::F32VEC2:
		case VertexAttribute::U32VEC2:
			return 32 * 2;
		case VertexAttribute::I32VEC3:
		case VertexAttribute::F32VEC3:
		case VertexAttribute::U32VEC3:
			return 32 * 3;
		case VertexAttribute::U32VEC4:
		case VertexAttribute::F32VEC4:
		case VertexAttribute::I32VEC4:
			return 32 * 4;

		case VertexAttribute::F64VEC2:
			return 64 * 2;
		case VertexAttribute::F64VEC3:
			return 64 * 3;
		case VertexAttribute::F64VEC4:
			return 64 * 4;
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
		case VertexAttribute::F32VEC2:
			return VK_FORMAT_R32G32_UINT;
		case VertexAttribute::U32VEC2:
			return VK_FORMAT_R32G32_SFLOAT;

		case VertexAttribute::I32VEC3:
			return VK_FORMAT_R32G32B32_SINT;
		case VertexAttribute::F32VEC3:
			return VK_FORMAT_R32G32B32_UINT;
		case VertexAttribute::U32VEC3:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case VertexAttribute::U32VEC4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case VertexAttribute::F32VEC4:
			return VK_FORMAT_R32G32B32A32_UINT;
		case VertexAttribute::I32VEC4:
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
