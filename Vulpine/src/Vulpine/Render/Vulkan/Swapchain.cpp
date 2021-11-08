#include "vppch.h"
#include "Swapchain.h"

#include "Core/Window.h"
#include "Device.h"

namespace Vulpine::Vulkan
{
	void Swapchain::Create()
	{
		s_Swapchain = VK_NULL_HANDLE;

		SetupSwapchain();
	}

	void Swapchain::Recreate()
	{
		Destroy();
		SetupSwapchain();
	}

	void Swapchain::Destroy()
	{
		for (const auto& imageView : s_ImageViews)
		{
			vkDestroyImageView(Device::GetLogicalDevice(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(Device::GetLogicalDevice(), s_Swapchain, nullptr);
	}

	void Swapchain::SetupSwapchain()
	{
		Device::PhysicalDeviceInfo queueIndices = Device::QueryPhysicalDeviceInfo(Device::GetPhysicalDevice());
		Swapchain::SwapchainSupportInfo supportInfo = Swapchain::QuerySwapchainSupportInfo();

		VkExtent2D framebufferSize;

		// Select the best matching framebuffer size which is supported
		{
			if (supportInfo.surfaceCapabilities.currentExtent.width != UINT32_MAX)
			{
				framebufferSize = supportInfo.surfaceCapabilities.currentExtent;
			}
			else
			{
				framebufferSize = Window::QueryFramebufferSize();

				// Ensure the iamge extent is supported

				framebufferSize.width = std::clamp(framebufferSize.width,
					supportInfo.surfaceCapabilities.minImageExtent.width, supportInfo.surfaceCapabilities.maxImageExtent.width
				);
				framebufferSize.height = std::clamp(framebufferSize.height,
					supportInfo.surfaceCapabilities.minImageExtent.height, supportInfo.surfaceCapabilities.maxImageExtent.height
				);
			}
		}

		VkSurfaceFormatKHR surfaceFormat;

		// Select the best supported surface format
		{
			for (const auto& supportedSurfaceFormat : supportInfo.supportedSurfaceFormats)
			{
				if (supportedSurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
					surfaceFormat = supportedSurfaceFormat; // Optimal surface format
			}

			surfaceFormat = supportInfo.supportedSurfaceFormats[0];
		}

		VkPresentModeKHR presentMode;

		// Select the best supported present mode
		{
			for (const auto& supportedPresentMode : supportInfo.supportedPresentModes)
			{
				if (supportedPresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
					presentMode = supportedPresentMode; // Optimal present mode
			}

			presentMode = VK_PRESENT_MODE_FIFO_KHR; // FIFO is always supported
		}

		uint32_t imageCount = supportInfo.surfaceCapabilities.minImageCount + 1;

		if (supportInfo.surfaceCapabilities.maxImageCount > 0 && imageCount > supportInfo.surfaceCapabilities.maxImageCount)
			imageCount = supportInfo.surfaceCapabilities.maxImageCount;

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = Window::GetSurface();

		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = framebufferSize;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Select correct iamge sharing mode

		if (queueIndices.GraphicsQueueIndex.value() != queueIndices.PresentQueueIndex.value())
		{
			uint32_t queueFamilyIndices[] = {
				queueIndices.GraphicsQueueIndex.value(),
				queueIndices.PresentQueueIndex.value()
			};

			swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = 2;
			swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0;
			swapchainInfo.pQueueFamilyIndices = nullptr;
		}

		swapchainInfo.preTransform = supportInfo.surfaceCapabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = VK_TRUE;

		swapchainInfo.oldSwapchain = s_Swapchain;

		VP_ASSERT_VK(vkCreateSwapchainKHR(Device::GetLogicalDevice(), &swapchainInfo, nullptr, &s_Swapchain), "Failed to create swapchain!");

		s_FramebufferSize = framebufferSize;
		s_ImageFormat = surfaceFormat.format;

		SetupImageViews();
	}

	void Swapchain::SetupImageViews()
	{
		uint32_t imageViewCount;
		vkGetSwapchainImagesKHR(Device::GetLogicalDevice(), s_Swapchain, &imageViewCount, nullptr);

		if (imageViewCount != 0)
		{
			s_Images.resize(imageViewCount);
			vkGetSwapchainImagesKHR(Device::GetLogicalDevice(), s_Swapchain, &imageViewCount, s_Images.data());
		}

		for (int i = 0; i < imageViewCount; i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = s_Images[i];

			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = s_ImageFormat;

			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			VP_ASSERT_VK(vkCreateImageView(Device::GetLogicalDevice(), &imageViewCreateInfo, nullptr, &s_ImageViews[i]), "Failed to create image views!");
		}
	}

	Swapchain::SwapchainSupportInfo Swapchain::QuerySwapchainSupportInfo()
	{
		SwapchainSupportInfo supportInfo;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device::GetPhysicalDevice(), Window::GetSurface(), &supportInfo.surfaceCapabilities);

		uint32_t surfaceFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device::GetPhysicalDevice(), Window::GetSurface(), &surfaceFormatCount, nullptr);
		if (surfaceFormatCount != 0)
		{
			supportInfo.supportedSurfaceFormats.resize(surfaceFormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(Device::GetPhysicalDevice(), Window::GetSurface(), &surfaceFormatCount, supportInfo.supportedSurfaceFormats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device::GetPhysicalDevice(), Window::GetSurface(), &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			supportInfo.supportedPresentModes.resize(surfaceFormatCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(Device::GetPhysicalDevice(), Window::GetSurface(), &presentModeCount, supportInfo.supportedPresentModes.data());
		}

		return supportInfo;
	}
}