#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "Vulpine/Vulpine.h"
#include "Vulpine/Core/Window.h"

// Use windowed app if it's in the Dist configuration
#ifdef VP_DIST
	#define MAIN WinMain
#else
	#define MAIN main
#endif

using namespace Vulpine;
using namespace Vulkan;

void MAIN()
{
	spdlog::set_pattern("[%^%l%$ %T] %s(%#): %v");
	spdlog::set_level(spdlog::level::trace);

	Window::Create("Mein gude fenster", 800, 600);

	Instance::Create();
	Swapchain::Create();

	Shader vertShader("src/shader.vert.spv", Shader::Type::VERTEX);
	Shader fragShader("src/shader.frag.spv", Shader::Type::FRAGMENT);

	VertexBuffer vertBuffer({
		VertexAttribute::F32VEC2
	});

	Pipeline pipeline(vertShader, fragShader);

	pipeline.Create();

	vertShader.Destroy();
	fragShader.Destroy();

	CommandPool::Create();
	CommandBuffers::Create();
	CommandBuffers::Record(pipeline);

	Semaphore imageAvailableSemaphore;
	Semaphore renderFinishedSemaphore;

	while (!glfwWindowShouldClose(Vulpine::Window::GetWindow()))
	{
		glfwWaitEvents();

		uint32_t imageIndex = Swapchain::AcquireNextImageIndex(imageAvailableSemaphore);
		CommandBuffers::Submit(imageIndex, imageAvailableSemaphore, renderFinishedSemaphore);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		VkSemaphore renderFinished = renderFinishedSemaphore.GetSemaphore();

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinished;

		VkSwapchainKHR swapChains[] = { Swapchain::GetSwapchain() };

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(Device::GetPresentQueue(), &presentInfo);
	}

	vkDeviceWaitIdle(Device::GetLogicalDevice());

	imageAvailableSemaphore.Destroy();
	renderFinishedSemaphore.Destroy();

	CommandBuffers::Destroy();
	CommandPool::Destroy();

	pipeline.Destroy();

	Swapchain::Destroy();
	Instance::Destory();

	Window::Destroy();
}