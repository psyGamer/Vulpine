#include "vppch.h"
#include "DataType.h"

namespace Vulpine::Vulkan
{
	uint32_t DataTypeHelper::QueryDataTypeSize(const DataType& vertexAttribute)
	{
		switch (vertexAttribute)
		{
			// Normal
		case DataType::INT8:
		case DataType::UINT8:
			return sizeof(int8_t);

		case DataType::INT16:
		case DataType::UINT16:
			return sizeof(int16_t);

		case DataType::INT32:
		case DataType::UINT32:
		case DataType::FLOAT32:
			return sizeof(int32_t);

		case DataType::INT64:
		case DataType::UINT64:
		case DataType::FLOAT64:
			return sizeof(int64_t);

			// Vector
		case DataType::I32VEC2:
		case DataType::F32VEC2:
		case DataType::U32VEC2:
			return 4 * 2;
		case DataType::I32VEC3:
		case DataType::U32VEC3:
		case DataType::F32VEC3:
			return 4 * 3;
		case DataType::I32VEC4:
		case DataType::U32VEC4:
		case DataType::F32VEC4:
			return 4 * 4;

		case DataType::F64VEC2:
			return 8 * 2;
		case DataType::F64VEC3:
			return 8 * 3;
		case DataType::F64VEC4:
			return 8 * 4;
		}
	}

	VkFormat DataTypeHelper::QueryDataTypeFormat(const DataType& dataType)
	{
		switch (dataType)
		{
		case DataType::INT8:
			return VK_FORMAT_R8_SINT;
		case DataType::UINT8:
			return VK_FORMAT_R8_UINT;

		case DataType::INT16:
			return VK_FORMAT_R16_SINT;
		case DataType::UINT16:
			return VK_FORMAT_R16_UINT;

		case DataType::INT32:
			return VK_FORMAT_R32_SINT;
		case DataType::UINT32:
			return VK_FORMAT_R32_UINT;

		case DataType::FLOAT32:
			return VK_FORMAT_R32_SFLOAT;
		case DataType::FLOAT64:
			return VK_FORMAT_R64_SFLOAT;

		case DataType::I32VEC2:
			return VK_FORMAT_R32G32_SINT;
		case DataType::U32VEC2:
			return VK_FORMAT_R32G32_UINT;
		case DataType::F32VEC2:
			return VK_FORMAT_R32G32_SFLOAT;

		case DataType::I32VEC3:
			return VK_FORMAT_R32G32B32_SINT;
		case DataType::U32VEC3:
			return VK_FORMAT_R32G32B32_UINT;
		case DataType::F32VEC3:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case DataType::I32VEC4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case DataType::U32VEC4:
			return VK_FORMAT_R32G32B32A32_UINT;
		case DataType::F32VEC4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case DataType::F64VEC2:
			return VK_FORMAT_R64G64_SFLOAT;
		case DataType::F64VEC3:
			return VK_FORMAT_R64G64B64_SFLOAT;
		case DataType::F64VEC4:
			return VK_FORMAT_R64G64B64A64_SFLOAT;
		}
	}
}