#pragma once

#include <optional>

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Instance;

	class Device
	{
	public:
		Device(const Reference<VkInstance> instance, const Reference<VkSurfaceKHR> surface);
		~Device();

	private:
		struct PhysicalDeviceInfo
		{
			const VkPhysicalDevice PhysicalDevice;

			std::optional<uint32_t> GraphicsQueueIndex;
			std::optional<uint32_t> PresentQueueIndex;

			bool QueueIndicesFound() const
			{
				return GraphicsQueueIndex.has_value() && PresentQueueIndex.has_value();
			}
		};

		static const PhysicalDeviceInfo& ChoseOptimalPhysicalDevice(const Reference<VkInstance> instance, const Reference<VkSurfaceKHR> surface);
		static uint32_t RatePhysicalDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const Reference<VkSurfaceKHR> surface);

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
	};
}

