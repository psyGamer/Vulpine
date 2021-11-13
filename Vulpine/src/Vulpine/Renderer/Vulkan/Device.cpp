#include "vppch.h"
#include "Device.h"

#include <map>

#include "Core/Window.h"

namespace Vulpine::Vulkan
{
	Device::PhysicalDeviceInfo Device::s_PhysicalDeviceInfo = {};

	VkDevice Device::s_LogicalDevice = VK_NULL_HANDLE;
	VkPhysicalDevice Device::s_PhysicalDevice = VK_NULL_HANDLE;

	VkQueue Device::s_GraphicsQueue = VK_NULL_HANDLE;
	VkQueue Device::s_PresentQueue = VK_NULL_HANDLE;

	void Device::Create(const VkInstance& instance)
	{
		const PhysicalDeviceInfo physicalDeviceInfo = ChoseOptimalPhysicalDevice(instance);

		s_PhysicalDevice = physicalDeviceInfo.PhysicalDevice;

		// Setup queues
		std::vector<VkDeviceQueueCreateInfo> queueInfos;
		
		{
			// Prevent queue duplication when the graphics and present are the same
			const std::unordered_set<uint32_t> uniqueQueueIndices = {
				physicalDeviceInfo.GraphicsQueueIndex.value(),
				physicalDeviceInfo.PresentQueueIndex.value()
			};

			for (const uint32_t queueIndex : uniqueQueueIndices)
			{
				constexpr float queuePriority = 1.0f;

				VkDeviceQueueCreateInfo queueInfo{};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

				queueInfo.queueFamilyIndex = queueIndex;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;

				queueInfos.push_back(queueInfo);
			}
		}

		// Create logical device
		VkPhysicalDeviceFeatures deviceFeatures{};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
		deviceInfo.pQueueCreateInfos = queueInfos.data();

		deviceInfo.enabledLayerCount = 0;
		deviceInfo.ppEnabledLayerNames = nullptr;
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

		deviceInfo.pEnabledFeatures = &deviceFeatures;

		VP_ASSERT_VK(vkCreateDevice(s_PhysicalDevice, &deviceInfo, nullptr, &s_LogicalDevice), "Failed to create logical device!");

		vkGetDeviceQueue(s_LogicalDevice, physicalDeviceInfo.GraphicsQueueIndex.value(), 0, &s_GraphicsQueue);
		vkGetDeviceQueue(s_LogicalDevice, physicalDeviceInfo.PresentQueueIndex.value(), 0, &s_PresentQueue);
	}

	void Device::Destory()
	{
		vkDestroyDevice(s_LogicalDevice, nullptr);
	}

	const Device::PhysicalDeviceInfo& Device::ChoseOptimalPhysicalDevice(const VkInstance& instance)
	{
		std::vector<VkPhysicalDevice> physicalDevices;

		// Get all physical devices
		{
			uint32_t physicalDeviceCount;
			vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

			VP_ASSERT(physicalDeviceCount == 0, "Could not find a GPU which supports Vulkan!");

			physicalDevices.resize(physicalDeviceCount);
			vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
		}

		// Rate the physical devices based on their features and properties
		uint32_t topScore = 0;
		std::multimap<uint32_t, PhysicalDeviceInfo> ratedPhysicalDevices;

		for (auto physicalDevice : physicalDevices)
		{
			PhysicalDeviceInfo physicalDeviceInfo = QueryPhysicalDeviceInfo(physicalDevice);

			uint32_t score = RatePhysicalDevice(physicalDeviceInfo);

			if (score <= topScore)
				continue;

			// Check if This device supports all required queue families
			if (!physicalDeviceInfo.QueueIndicesFound())
				continue;

			topScore = score;
			ratedPhysicalDevices.insert(std::make_pair(score, physicalDeviceInfo));
		}

		// Verify that the best device is even supported
		VP_ASSERT(ratedPhysicalDevices.rbegin()->first == 0, "Could not find a GPU which supports Vulkan!");

		return ratedPhysicalDevices.rbegin()->second;
	}

	Device::PhysicalDeviceInfo Device::QueryPhysicalDeviceInfo(const VkPhysicalDevice& physicalDevice)
	{
		PhysicalDeviceInfo physicalDeviceInfo = { physicalDevice };

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		for (int i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				physicalDeviceInfo.GraphicsQueueIndex = i;

			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, Window::GetSurface(), &presentSupport);

			if (presentSupport)
				physicalDeviceInfo.PresentQueueIndex = i;

			// All required queue indices have been found
			if (physicalDeviceInfo.QueueIndicesFound())
				break;
		}

		return physicalDeviceInfo;
	}

	uint32_t Device::RatePhysicalDevice(const Device::PhysicalDeviceInfo& physicalDeviceInfo)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDeviceInfo.PhysicalDevice, &deviceProperties);

		if (!physicalDeviceInfo.QueueIndicesFound())
			return 0; // A score of 0 means this device isn't supported

		// Check if this device supports all required extensions
		{
			uint32_t extensionPropertyCount;
			vkEnumerateDeviceExtensionProperties(physicalDeviceInfo.PhysicalDevice, nullptr, &extensionPropertyCount, nullptr);
			std::vector<VkExtensionProperties> extentionProperties(extensionPropertyCount);
			vkEnumerateDeviceExtensionProperties(physicalDeviceInfo.PhysicalDevice, nullptr, &extensionPropertyCount, extentionProperties.data());

			std::unordered_set<std::string> requiredExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			for (const auto& extensionProperty : extentionProperties)
			{
				requiredExtensions.erase(extensionProperty.extensionName);

				if (requiredExtensions.empty())
					break;
			}

			if (!requiredExtensions.empty())
				return 0;
		}

		uint32_t score = 0;

		// Dedicated GPUs are almost always better than integrated ones
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		score += deviceProperties.limits.maxImageDimension2D;
		// The max push constant size is doubled in order to make an actual impact to the score
		score += deviceProperties.limits.maxPushConstantsSize * 2;

		return score;
	}
}
