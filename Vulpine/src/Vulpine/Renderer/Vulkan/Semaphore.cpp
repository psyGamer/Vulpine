#include "vppch.h"
#include "Semaphore.h"

#include "Device.h"

namespace Vulpine::Vulkan
{
	Semaphore::Semaphore()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VP_ASSERT_VK(vkCreateSemaphore(Device::GetLogicalDevice(), &semaphoreInfo, nullptr, &m_Semaphore), "Failed to create semaphore!");
	}

	Semaphore::~Semaphore()
	{
		Destroy();
	}

	void Semaphore::Destroy()
	{
		vkDestroySemaphore(Device::GetLogicalDevice(), m_Semaphore, nullptr);
	}
}