#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Swapchain
	{
	public:
		static void Create();
		static void Recreate();
		static void Destroy();

		static const VkSwapchainKHR& GetSwapchain() { return s_Swapchain; }

		static const std::vector<VkImage>& GetImages() { return s_Images; }
		static const std::vector<VkImageView>& GetImageViews() { return s_ImageViews; }

	private:
		struct SwapchainSupportInfo
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;

			std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
			std::vector<VkPresentModeKHR> supportedPresentModes;
		};

		static void SetupSwapchain();
		static void SetupImageViews();

		static SwapchainSupportInfo QuerySwapchainSupportInfo();

	private:
		static VkSwapchainKHR s_Swapchain;

		static std::vector<VkImage> s_Images;
		static std::vector<VkImageView> s_ImageViews;

		static VkExtent2D s_FramebufferSize;
		static VkFormat s_ImageFormat;
	};
}
