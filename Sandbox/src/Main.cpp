/*
 *	IMPORTANT NOTE
 *
 *	This Sandbox project is just me playing around with the features of the engine.
 *	You may use this as an example, but it's most certainly not intended as one.
 *
 *	I am aware that the code is "bad", however since this is just a playground I really don't care.
 */

#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

#include "Vulpine/Vulpine.h"
#include "Vulpine/Core/Window.h"
#include "Vulpine/Core/Log.h"
#include "Vulpine/Core/Assert.h"

// Use windowed app if it's in the Dist configuration
#ifdef VP_DIST
	#define MAIN WinMain
#else
	#define MAIN main
#endif

using namespace Vulpine;
using namespace Vulkan;

struct MultiColor
{ // Make sure every attribute is aligned to 16 bytes
	alignas(16) glm::vec3 ColorA;
	alignas(16) glm::vec3 ColorB;
	alignas(16) glm::vec3 ColorC;
	alignas(16) glm::vec3 ColorD;
};

#define RNGCOLORCOMP ((float)rand() / RAND_MAX)
#define RNGCOLOR glm::vec3{ RNGCOLORCOMP, RNGCOLORCOMP, RNGCOLORCOMP}

#define CMOD

#ifdef CMOD
	#define CLAMPCOLORCOMP(colorComp) colorComp = (float)((int32_t)(colorComp * 255.0f) % 255) / 255
	#define CLAMPCOLOR(color) CLAMPCOLORCOMP(color.r); CLAMPCOLORCOMP(color.g); CLAMPCOLORCOMP(color.b); 
#else
	#define CLAMPCOLORCOMP(colorComp)
	#define CLAMPCOLOR(color)
#endif

void MAIN()
{
	spdlog::set_pattern("[%^%l%$ %T] %s(%#): %v");
	spdlog::set_level(spdlog::level::trace);

	Window::Create("Mein gude fenster", 800, 600);

	Instance::Create();
	Swapchain::Create();

	Shader vertShader("src/shader.vert.spv", Shader::Type::VERTEX);
	Shader fragShader("src/shader.frag.spv", Shader::Type::FRAGMENT);

	{
		DescriptorPool::Create();

		VertexBuffer vertBuffer({
			DataType::F32VEC2,
#ifdef VCOLOR
			DataType::F32VEC3
#endif
		}, 4);
		IndexBuffer indexBuffer(6);
		UniformBuffer uniBuffer(sizeof(MultiColor));
#ifdef VCOLOR
		const float positions[] = {
			// Position	  // Color
			-0.5f, -0.5f, 0.75f, 0.50f, 0.00f, // Top Left
			 0.5f, -0.5f, 0.75f, 0.00f, 0.50f, // Top Right
			 0.5f,  0.5f, 0.00f, 0.75f, 0.50f, // Bottom Right
			-0.5f,  0.5f, 0.50f, 0.00f, 0.75f, // Bottom Left
		};
#else
		const float positions[] = {
			// Position	
			-0.5f, -0.5f, // Top Left
			 0.5f, -0.5f, // Top Right
			 0.5f,  0.5f, // Bottom Right
			-0.5f,  0.5f, // Bottom Left
		};
#endif
		const uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		//const glm::vec3 uni = glm::vec3{ 0.25f, 0.6f, 0.8f };

		Pipeline pipeline(vertShader, fragShader);
		pipeline.SetVertexBuffer(vertBuffer);
		pipeline.SetIndexBuffer(indexBuffer);
		pipeline.SetUniformBuffer(uniBuffer);
		pipeline.Create();

		vertShader.Destroy();
		fragShader.Destroy();

		GraphicsCommandPool::Create();
		TransferCommandPool::Create();

		GraphicsCommandBuffer commandBuffer;

		vertBuffer.SetData(&positions);
		indexBuffer.SetData(&indices);

		commandBuffer.Record(pipeline);

		Semaphore imageAvailableSemaphore;
		Semaphore renderFinishedSemaphore;

#if 0
		MultiColor uniColor
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f }
		};
#else
		MultiColor uniColor = {
			RNGCOLOR,
			RNGCOLOR,
			RNGCOLOR,
			RNGCOLOR
		};
#endif

		uniBuffer.SetData(&uniColor);

		srand(time(NULL));

		while (!glfwWindowShouldClose(Vulpine::Window::GetWindow()))
		{
			// Update
#if 1
			if ((int32_t)((float)rand() / RAND_MAX * 3000) == 0)
			{
				uniColor = {
					RNGCOLOR,
					RNGCOLOR,
					RNGCOLOR,
					RNGCOLOR
				};
			}

			const float sensitivity = 0.025f;

			uniColor.ColorA += RNGCOLOR * sensitivity;
			CLAMPCOLOR(uniColor.ColorA);
			uniColor.ColorB += RNGCOLOR * sensitivity;
			CLAMPCOLOR(uniColor.ColorB);
			uniColor.ColorC += RNGCOLOR * sensitivity;
			CLAMPCOLOR(uniColor.ColorC);
			uniColor.ColorD += RNGCOLOR * sensitivity;
			CLAMPCOLOR(uniColor.ColorD);

			uniBuffer.SetData(&uniColor);
#endif
			// Render
			//glfwWaitEvents();
			glfwPollEvents();

			uint32_t imageIndex = Swapchain::AcquireNextImageIndex(imageAvailableSemaphore);
			commandBuffer.Submit(imageIndex, &imageAvailableSemaphore, &renderFinishedSemaphore);

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

		commandBuffer.~GraphicsCommandBuffer();
		DescriptorPool::Destroy();
		GraphicsCommandPool::Destroy();
		TransferCommandPool::Destroy();

		pipeline.Destroy();
	}

	Swapchain::Destroy();
	Instance::Destory();

	Window::Destroy();
}