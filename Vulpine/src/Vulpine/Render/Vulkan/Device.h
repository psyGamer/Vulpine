#pragma once

#include <optional>

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Instance;

	class Device
	{
	public:
		static void Create(const VkInstance& instance);
		static void Destory();

		static const VkDevice& GetLogicalDevice();
		static const VkPhysicalDevice& GetPhysicalDevice();

		static const VkQueue& GetGraphicsQueue();
		static const VkQueue& GetPresentQueue();

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

		static const PhysicalDeviceInfo& ChoseOptimalPhysicalDevice(const VkInstance& instance);
		static PhysicalDeviceInfo QueryPhysicalDeviceInfo(const VkPhysicalDevice& physicalDevice);
		static uint32_t RatePhysicalDevice(const PhysicalDeviceInfo& physicalDeviceInfo);

	private:
		static VkDevice s_LogicalDevice;
		static VkPhysicalDevice s_PhysicalDevice;

		static VkQueue s_GraphicsQueue;
		static VkQueue s_PresentQueue;

		friend class Swapchain;
	};
}

