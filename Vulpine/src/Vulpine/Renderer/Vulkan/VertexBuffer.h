#pragma once

#include "Vulkan/Vulkan.h"

#include "Buffer.h"
#include "DataType.h"

namespace Vulpine::Vulkan
{
	class VertexBuffer
	{
	public:

		VertexBuffer();
		VertexBuffer(const std::initializer_list<DataType>& vertexAttributes, uint32_t vertexCount);

		void SetData(const void* const data);
		void SetLayout(const std::initializer_list<DataType>& vertexAttributes, uint32_t vertexCount);

		inline VkBuffer GetBuffer() const { return m_pBuffer->GetBuffer(); }
		inline uint32_t GetVertexCount() const { return m_VertexCount; }

	private:
		VkVertexInputBindingDescription QueryBindingDescriptions(uint32_t bindingIndex) const;
		std::vector<VkVertexInputAttributeDescription> QueryAttributeDescriptions(uint32_t bindingIndex) const;

	public:
		std::shared_ptr<Buffer> m_pBuffer;

		uint32_t m_VertexCount;
		std::vector<DataType> m_VertexAttributes;

		friend class Pipeline;
	};
}
