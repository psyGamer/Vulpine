#pragma onc

#include "Vulkan/Vulkan.h"

namespace Vulpine
{
	namespace Vulkan
	{
		class Pipeline;
	}

	class VertexAttribute
	{
	public:
		enum class ValueType
		{
			INT8 = VK_FORMAT_R8_SINT,
			UINT32 = VK_FORMAT_R8_UINT,
			INT16 = VK_FORMAT_R16_SINT,
			UINT16 = VK_FORMAT_R16_UINT,
			INT32 = VK_FORMAT_R32_SINT,
			UINT32 = VK_FORMAT_R32_UINT,

			FLOAT = VK_FORMAT_R32_SFLOAT,
			DOUBLE = VK_FORMAT_R64_SFLOAT,

			VEC2 = VK_FORMAT_R32G32_SFLOAT,
			VEC3 = VK_FORMAT_R32G32B32_SFLOAT,
			VEC4 = VK_FORMAT_R32G32B32A32_SFLOAT,

			IVEC2 = VK_FORMAT_R32G32_SINT,
			IVEC3 = VK_FORMAT_R32G32B32_SINT,
			IVEC4 = VK_FORMAT_R32G32B32A32_SINT,

			UVEC2 = VK_FORMAT_R32G32_UINT,
			UVEC3 = VK_FORMAT_R32G32B32_UINT,
			UVEC4 = VK_FORMAT_R32G32B32A32_UINT,

			DVEC2 = VK_FORMAT_R64G64_SFLOAT,
			DVEC3 = VK_FORMAT_R64G64B64_SFLOAT,
			DVEC4 = VK_FORMAT_R64G64B64A64_SFLOAT
		};

		VertexAttribute(ValueType valueType, uint32_t offset);

		ValueType GetValueType() const { return m_ValueType; }
		uint32_t GetOffset() const { return m_Offset; }

	private:
		ValueType m_ValueType;
		uint32_t m_Offset;
	};

	class VertexBuffer
	{
	public:
		void AddAttribute(const VertexAttribute& attribute);

	private:
		const std::vector<VkVertexInputBindingDescription>& QueryBindingDescriptions();
		const std::vector<VkVertexInputAttributeDescription>& QueryAttributeDescriptions();

	private:
		std::vector<VertexAttribute> m_VertexAttributes;

		friend Vulkan::Pipeline;
	};


}