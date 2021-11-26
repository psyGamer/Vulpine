#pragma once

#include "Vulkan/Vulkan.h"

#include "Buffer.h"

namespace Vulpine::Vulkan
{
	class VertexBuffer
	{
	public:

		VertexBuffer();
		VertexBuffer(const std::initializer_list<DataType>& vertexAttributes, uint32_t vertexCount);

		void SetData(const void* const data);
		void SetLayout(const std::initializer_list<DataType>& vertexAttributes, uint32_t vertexCount);

		VkBuffer GetBuffer() const { return m_pBuffer->GetBuffer(); }

		uint32_t GetVertexCount() const { return m_VertexCount; }
	private:
		VkVertexInputBindingDescription QueryBindingDescriptions(uint32_t bindingIndex) const;
		void QueryAttributeDescriptions(uint32_t bindingIndex, std::vector<VkVertexInputAttributeDescription>* attributeDescriptionBuffer) const;

	public:
		Reference<Buffer> m_pBuffer;

		uint32_t m_VertexCount;
		std::vector<DataType> m_VertexAttributes;

		friend class Pipeline;
	};
}
