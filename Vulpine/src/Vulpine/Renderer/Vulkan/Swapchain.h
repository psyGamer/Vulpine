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
		static const std::vector<VkFramebuffer>& GetFramebuffers() { return s_Framebuffers; }

	private:
		struct SwapchainSupportInfo
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;

			std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
			std::vector<VkPresentModeKHR> supportedPresentModes;
		};

		static void SetupSwapchain();
		static void SetupImageViews();
		static void SetupFramebuffers();

		static SwapchainSupportInfo QuerySwapchainSupportInfo();

	private:
		static VkSwapchainKHR s_Swapchain;

		static std::vector<VkImage> s_Images;
		static std::vector<VkImageView> s_ImageViews;
		static std::vector<VkFramebuffer> s_Framebuffers;

		static VkExtent2D s_FramebufferSize;
		static VkFormat s_ImageFormat;

		friend class RenderPass;
	};
}
