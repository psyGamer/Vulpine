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

	private:
		struct SwapchainSupportInfo
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;

			std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
			std::vector<VkPresentModeKHR> supportedPresentModes;
		};

		static void SetupSwapchain();

		static SwapchainSupportInfo QuerySwapchainSupportInfo();

	private:
		static VkSwapchainKHR s_Swapchain;

		static VkExtent2D s_FramebufferSize;
		static VkFormat s_ImageFormat;
	};
}
