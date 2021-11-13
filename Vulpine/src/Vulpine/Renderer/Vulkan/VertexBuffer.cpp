#include "vppch.h"
#include "VertexBuffer.h"

namespace Vulpine::Vulkan
{
	VertexBuffer::VertexBuffer(uint32_t bindingIndex)
		: m_BindingIndex(bindingIndex)
	{ }
	VertexBuffer::VertexBuffer(uint32_t bindingIndex, std::initializer_list<VertexAttributeValue> vertexAttributes)
		: m_BindingIndex(bindingIndex), m_VertexAttributes(vertexAttributes)
	{ }

	void VertexBuffer::AddAttribute(VertexAttributeValue vertexAttribute)
	{
		m_VertexAttributes.push_back(vertexAttribute);
	}

	VkVertexInputBindingDescription VertexBuffer::QueryBindingDescriptions()
	{
		uint32_t stride = 0;

		for (const auto& vertexAttribute : m_VertexAttributes)
			stride += QueryVertexAttributeSize(vertexAttribute);

		VkVertexInputBindingDescription bindingDescription;
		bindingDescription.binding = m_BindingIndex;
		bindingDescription.stride = stride;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	const std::vector<VkVertexInputAttributeDescription>& VertexBuffer::QueryAttributeDescriptions()
	{
		uint32_t location = 0, offset = 0;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		attributeDescriptions.reserve(m_VertexAttributes.size());

		for (const auto& vertexAttribute : m_VertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription = attributeDescriptions.emplace_back();
			attributeDescription.location = location++;
			attributeDescription.binding = offset;
			attributeDescription.format = QueryVertexAttributeFormat(vertexAttribute);
			attributeDescription.offset = offset;

			offset += QueryVertexAttributeSize(vertexAttribute);
		}

		return attributeDescriptions;
	}

	uint32_t VertexBuffer::QueryVertexAttributeSize(const VertexAttributeValue& vertexAttribute)
	{
		switch (vertexAttribute)
		{
		case VertexAttributeValue::INT8:
		case VertexAttributeValue::UINT8:
			return 8;
			
		case VertexAttributeValue::INT16:
		case VertexAttributeValue::UINT16:
			return 16;

		case VertexAttributeValue::INT32:
		case VertexAttributeValue::UINT32:
		case VertexAttributeValue::FLOAT32:
			return 32;

		case VertexAttributeValue::DOUBLE64:
			return 64;

		case VertexAttributeValue::I32VEC2:
		case VertexAttributeValue::F32VEC2:
		case VertexAttributeValue::U32VEC2:
			return 32 * 2;
		case VertexAttributeValue::I32VEC3:
		case VertexAttributeValue::F32VEC3:
		case VertexAttributeValue::U32VEC3:
			return 32 * 3;
		case VertexAttributeValue::U32VEC4:
		case VertexAttributeValue::F32VEC4:
		case VertexAttributeValue::I32VEC4:
			return 32 * 4;

		case VertexAttributeValue::D64VEC2:
			return 64 * 2;
		case VertexAttributeValue::D64VEC3:
			return 64 * 3;
		case VertexAttributeValue::D64VEC4:
			return 64 * 4;
		}
	}

	VkFormat VertexBuffer::QueryVertexAttributeFormat(const VertexAttributeValue& vertexAttribute)
	{
		switch (vertexAttribute)
		{
		case VertexAttributeValue::INT8:
			return VK_FORMAT_R8_SINT;
		case VertexAttributeValue::UINT8:
			return VK_FORMAT_R8_UINT;

		case VertexAttributeValue::INT16:
			return VK_FORMAT_R16_SINT;
		case VertexAttributeValue::UINT16:
			return VK_FORMAT_R16_UINT;

		case VertexAttributeValue::INT32:
			return VK_FORMAT_R32_SINT;
		case VertexAttributeValue::UINT32:
			return VK_FORMAT_R32_UINT;
		case VertexAttributeValue::FLOAT32:
			return VK_FORMAT_R32_SFLOAT;

		case VertexAttributeValue::DOUBLE64:
			return VK_FORMAT_R64_SFLOAT;

		case VertexAttributeValue::I32VEC2:
			return VK_FORMAT_R32G32_SINT;
		case VertexAttributeValue::F32VEC2:
			return VK_FORMAT_R32G32_UINT;
		case VertexAttributeValue::U32VEC2:
			return VK_FORMAT_R32G32_SFLOAT;

		case VertexAttributeValue::I32VEC3:
			return VK_FORMAT_R32G32B32_SINT;
		case VertexAttributeValue::F32VEC3:
			return VK_FORMAT_R32G32B32_UINT;
		case VertexAttributeValue::U32VEC3:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case VertexAttributeValue::U32VEC4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case VertexAttributeValue::F32VEC4:
			return VK_FORMAT_R32G32B32A32_UINT;
		case VertexAttributeValue::I32VEC4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case VertexAttributeValue::D64VEC2:
			return VK_FORMAT_R64G64_SFLOAT;
		case VertexAttributeValue::D64VEC3:
			return VK_FORMAT_R64G64B64_SFLOAT;
		case VertexAttributeValue::D64VEC4:
			return VK_FORMAT_R64G64B64A64_SFLOAT;
		}
	}
}
