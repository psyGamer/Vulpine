#pragma once

#include "Assert.h"
#include "Log.h"

#include <memory>

namespace Vulpine
{
	template<typename T>
	using Reference = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Reference<T> CreateReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	};
}
