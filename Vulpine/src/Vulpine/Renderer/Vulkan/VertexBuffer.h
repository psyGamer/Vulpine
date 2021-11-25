#pragma once

#include "Vulkan/Vulkan.h"

#include "Buffer.h"

namespace Vulpine::Vulkan
{
	enum class VertexAttribute
	{
		INT8, INT16, INT32, INT64,
		UINT8, UINT16, UINT32, UINT64,

		FLOAT32, FLOAT64,

		I32VEC2, I32VEC3, I32VEC4,
		U32VEC2, U32VEC3, U32VEC4,

		F32VEC2, F32VEC3, F32VEC4,
		F64VEC2, F64VEC3, F64VEC4
	};

	class VertexBuffer
	{
	public:

		VertexBuffer();
		VertexBuffer(const std::initializer_list<VertexAttribute>& vertexAttributes, uint32_t vertexCount);

		void SetLayout(const std::initializer_list<VertexAttribute>& vertexAttributes, uint32_t vertexCount);

		void SetData(const void* const data);

		VkBuffer GetBuffer() const { return m_pBuffer->GetBuffer(); }
	private:
		VkVertexInputBindingDescription QueryBindingDescriptions(uint32_t bindingIndex) const;
		void QueryAttributeDescriptions(uint32_t bindingIndex, std::vector<VkVertexInputAttributeDescription>* attributeDescriptionBuffer) const;

		static uint32_t QueryVertexAttributeSize(const VertexAttribute& vertexAttribute);
		static VkFormat QueryVertexAttributeFormat(const VertexAttribute& vertexAttribute);

	public:
		Reference<Buffer> m_pBuffer;

		std::vector<VertexAttribute> m_VertexAttributes;

		friend class Pipeline;
	};
}
