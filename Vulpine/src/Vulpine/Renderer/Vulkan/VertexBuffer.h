#pragma onc

#include "Vulkan/Vulkan.h"

namespace Vulpine::Vulkan
{
	enum class VertexAttributeValue
	{
		INT8 = VK_FORMAT_R8_SINT,
		UINT8 = VK_FORMAT_R8_UINT,
		INT16 = VK_FORMAT_R16_SINT,
		UINT16 = VK_FORMAT_R16_UINT,
		INT32 = VK_FORMAT_R32_SINT,
		UINT32 = VK_FORMAT_R32_UINT,

		FLOAT32 = VK_FORMAT_R32_SFLOAT,
		DOUBLE64 = VK_FORMAT_R64_SFLOAT,

		F32VEC2 = VK_FORMAT_R32G32_SFLOAT,
		F32VEC3 = VK_FORMAT_R32G32B32_SFLOAT,
		F32VEC4 = VK_FORMAT_R32G32B32A32_SFLOAT,

		D64VEC2 = VK_FORMAT_R64G64_SFLOAT,
		D64VEC3 = VK_FORMAT_R64G64B64_SFLOAT,
		D64VEC4 = VK_FORMAT_R64G64B64A64_SFLOAT,

		I32VEC2 = VK_FORMAT_R32G32_SINT,
		I32VEC3 = VK_FORMAT_R32G32B32_SINT,
		I32VEC4 = VK_FORMAT_R32G32B32A32_SINT,

		U32VEC2 = VK_FORMAT_R32G32_UINT,
		U32VEC3 = VK_FORMAT_R32G32B32_UINT,
		U32VEC4 = VK_FORMAT_R32G32B32A32_UINT
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t bindingIndex);
		VertexBuffer(uint32_t bindingIndex, std::initializer_list<VertexAttributeValue> vertexAttributes);

		void AddAttribute(VertexAttributeValue vertexAttribute);

	private:
		VkVertexInputBindingDescription QueryBindingDescriptions();
		const std::vector<VkVertexInputAttributeDescription>& QueryAttributeDescriptions();

		static uint32_t QueryVertexAttributeSize(const VertexAttributeValue& vertexAttribute);
		static VkFormat QueryVertexAttributeFormat(const VertexAttributeValue& vertexAttribute);

	private:
		uint32_t m_BindingIndex;
		std::vector<VertexAttributeValue> m_VertexAttributes;

		friend class Pipeline;
	};
}
