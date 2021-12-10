#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Semaphore
	{
	public:
		Semaphore();
		~Semaphore();

		void Destroy();

		inline VkSemaphore GetSemaphore() const { return m_Semaphore; }

	private:
		VkSemaphore m_Semaphore;
	};
}

