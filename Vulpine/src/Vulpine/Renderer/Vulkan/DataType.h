#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	enum class DataType
	{
			// Normal
		INT8,  INT16,  INT32,  INT64,
		UINT8, UINT16, UINT32, UINT64,

		FLOAT32, FLOAT64,

			// Vector 2
		I8VEC2,  U8VEC2,
		I16VEC2, U16VEC2,
		I32VEC2, U32VEC2,
		I64VEC2, U64VEC2,

		F32VEC2, F64VEC2,

			// Vector 3
		I8VEC3,  U8VEC3,  
		I16VEC3, U16VEC3,
		I32VEC3, U32VEC3, 
		I64VEC3, U64VEC3, 

		F32VEC3, F64VEC3,

			 // Vector 4
		I8VEC4,  U8VEC4,
		I16VEC4, U16VEC4,
		I32VEC4, U32VEC4,
		I64VEC4, U64VEC4,

		F32VEC4, F64VEC4
		
	};

	class DataTypeHelper
	{
	public:
		static uint32_t QueryDataTypeSize(const DataType& dataType);
		static VkFormat QueryDataTypeFormat(const DataType& dataType);
	};
}

