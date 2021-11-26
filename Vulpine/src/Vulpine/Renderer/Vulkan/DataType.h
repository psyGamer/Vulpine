#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	enum class DataType
	{
		INT8, INT16, INT32, INT64,
		UINT8, UINT16, UINT32, UINT64,

		FLOAT32, FLOAT64,

		I32VEC2, I32VEC3, I32VEC4,
		U32VEC2, U32VEC3, U32VEC4,

		F32VEC2, F32VEC3, F32VEC4,
		F64VEC2, F64VEC3, F64VEC4
	};

	class DataTypeHelper
	{
	public:
		static uint32_t QueryDataTypeSize(const DataType& dataType);
		static VkFormat QueryDataTypeFormat(const DataType& dataType);
	};
}

