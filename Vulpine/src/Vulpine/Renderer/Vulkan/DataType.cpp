#include "vppch.h"
#include "DataType.h"

namespace Vulpine::Vulkan
{
	uint32_t DataTypeHelper::QueryDataTypeSize(DataType vertexAttribute)
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

			// Vector 2
		case DataType::I8VEC2:
		case DataType::U8VEC2:
			return sizeof(int8_t) * 2;

		case DataType::I16VEC2:
		case DataType::U16VEC2:
			return sizeof(int16_t) * 2;

		case DataType::I32VEC2:
		case DataType::F32VEC2:
		case DataType::U32VEC2:
			return sizeof(int32_t) * 2;

		case DataType::I64VEC2:
		case DataType::U64VEC2:
		case DataType::F64VEC2:
			return sizeof(int64_t) * 2;

			// Vector 3
		case DataType::I8VEC3:
		case DataType::U8VEC3:
			return sizeof(int8_t) * 3;

		case DataType::I16VEC3:
		case DataType::U16VEC3:
			return sizeof(int16_t) * 3;

		case DataType::I32VEC3:
		case DataType::F32VEC3:
		case DataType::U32VEC3:
			return sizeof(int32_t) * 3;

		case DataType::I64VEC3:
		case DataType::U64VEC3:
		case DataType::F64VEC3:
			return sizeof(int64_t) * 3;

			// Vector 4
		case DataType::I8VEC4:
		case DataType::U8VEC4:
			return sizeof(int8_t) * 4;

		case DataType::I16VEC4:
		case DataType::U16VEC4:
			return sizeof(int16_t) * 4;

		case DataType::I32VEC4:
		case DataType::F32VEC4:
		case DataType::U32VEC4:
			return sizeof(int32_t) * 4;

		case DataType::I64VEC4:
		case DataType::U64VEC4:
		case DataType::F64VEC4:
			return sizeof(int64_t) * 4;
		}
	}

	VkFormat DataTypeHelper::QueryDataTypeFormat(DataType dataType)
	{
		switch (dataType)
		{
			// Normal
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

		case DataType::INT64:
			return VK_FORMAT_R64_SINT;
		case DataType::UINT64:
			return VK_FORMAT_R64_UINT;
		case DataType::FLOAT64:
			return VK_FORMAT_R64_SFLOAT;

			// Vector 2
		case DataType::I8VEC2:
			return VK_FORMAT_R8G8_SINT;
		case DataType::U8VEC2:
			return VK_FORMAT_R8G8_UINT;

		case DataType::I16VEC2:
			return VK_FORMAT_R16G16_SINT;
		case DataType::U16VEC2:
			return VK_FORMAT_R16G16_UINT;

		case DataType::I32VEC2:
			return VK_FORMAT_R32G32_SINT;
		case DataType::U32VEC2:
			return VK_FORMAT_R32G32_UINT;
		case DataType::F32VEC2:
			return VK_FORMAT_R32G32_SFLOAT;

		case DataType::I64VEC2:
			return VK_FORMAT_R64G64_SINT;
		case DataType::U64VEC2:
			return VK_FORMAT_R64G64_UINT;
		case DataType::F64VEC2:
			return VK_FORMAT_R64G64_SFLOAT;

			// Vector 3
		case DataType::I8VEC3:
			return VK_FORMAT_R8G8B8_SINT;
		case DataType::U8VEC3:
			return VK_FORMAT_R8G8B8_UINT;

		case DataType::I16VEC3:
			return VK_FORMAT_R16G16B16_SINT;
		case DataType::U16VEC3:
			return VK_FORMAT_R16G16B16_UINT;

		case DataType::I32VEC3:
			return VK_FORMAT_R32G32B32_SINT;
		case DataType::U32VEC3:
			return VK_FORMAT_R32G32B32_UINT;
		case DataType::F32VEC3:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case DataType::I64VEC3:
			return VK_FORMAT_R64G64B64_SINT;
		case DataType::U64VEC3:
			return VK_FORMAT_R64G64B64_UINT;
		case DataType::F64VEC3:
			return VK_FORMAT_R64G64B64_SFLOAT;

			// Vector 4
		case DataType::I8VEC4:
			return VK_FORMAT_R8G8B8A8_SINT;
		case DataType::U8VEC4:
			return VK_FORMAT_R8G8B8A8_UINT;

		case DataType::I16VEC4:
			return VK_FORMAT_R16G16B16A16_SINT;
		case DataType::U16VEC4:
			return VK_FORMAT_R16G16B16A16_UINT;

		case DataType::I32VEC4:
			return VK_FORMAT_R32G32B32A32_SINT;
		case DataType::U32VEC4:
			return VK_FORMAT_R32G32B32A32_UINT;
		case DataType::F32VEC4:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case DataType::I64VEC4:
			return VK_FORMAT_R64G64B64A64_SINT;
		case DataType::U64VEC4:
			return VK_FORMAT_R64G64B64A64_UINT;
		case DataType::F64VEC4:
			return VK_FORMAT_R64G64B64A64_SFLOAT;

		default:
			VP_CRITICAL("Converting matrix data type to VkFormats is not yet supported!");
		}
	}
}