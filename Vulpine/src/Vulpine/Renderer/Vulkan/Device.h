#pragma once

#include <optional>

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Device
	{
	public:
		struct PhysicalDeviceInfo
		{
			VkPhysicalDevice PhysicalDevice;

			std::optional<uint32_t> GraphicsQueueIndex;
			std::optional<uint32_t> PresentQueueIndex;
			std::optional<uint32_t> TransferQueueIndex;

			bool QueueIndicesFound() const
			{
				return GraphicsQueueIndex.has_value() && 
					   PresentQueueIndex.has_value()  && 
					   TransferQueueIndex.has_value();
			}
		};

		static void Create(const VkInstance& instance);
		static void Destory();

		static const PhysicalDeviceInfo& GetPhysicalDeviceInfo() { return s_PhysicalDeviceInfo; }

		static VkDevice GetLogicalDevice() { return s_LogicalDevice; }
		static VkPhysicalDevice GetPhysicalDevice() { return s_PhysicalDevice; }

		static VkQueue GetGraphicsQueue() { return s_GraphicsQueue; }
		static VkQueue GetPresentQueue() { return s_PresentQueue; }
		static VkQueue GetTransferQueue() { return s_TransferQueue; }
	private:
		static PhysicalDeviceInfo ChoseOptimalPhysicalDevice(const VkInstance& instance);
		static PhysicalDeviceInfo QueryPhysicalDeviceInfo(const VkPhysicalDevice& physicalDevice);
		static uint32_t RatePhysicalDevice(const PhysicalDeviceInfo& physicalDeviceInfo);

	private:
		static PhysicalDeviceInfo s_PhysicalDeviceInfo;

		static VkDevice s_LogicalDevice;
		static VkPhysicalDevice s_PhysicalDevice;

		static VkQueue s_GraphicsQueue;
		static VkQueue s_PresentQueue;
		static VkQueue s_TransferQueue;

		friend class Swapchain;
	};
}

