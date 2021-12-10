#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Semaphore;

	class Swapchain
	{
	public:
		static void Create();
		static void Recreate();
		static void Destroy();

		static uint32_t AcquireNextImageIndex(const Semaphore& semaphore);

		inline static const VkSwapchainKHR& GetSwapchain() { return s_Swapchain; }

		inline static const std::vector<VkImage>& GetImages() { return s_Images; }
		inline static const std::vector<VkImageView>& GetImageViews() { return s_ImageViews; }
		inline static const std::vector<VkFramebuffer>& GetFramebuffers() { return s_Framebuffers; }

		inline static VkExtent2D GetFramebufferSize() { return s_FramebufferSize; }
		inline static VkFormat GetImageFormat() { return s_ImageFormat; }

	private:
		struct SwapchainSupportInfo
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;

			std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
			std::vector<VkPresentModeKHR> supportedPresentModes;
		};

		Swapchain();

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
	};
}
