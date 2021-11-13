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
	Shader fragShader("src/shader.frag.spv", Shader::Type::VERTEX);

	VertexBuffer vertBuffer(0, {
		VertexAttributeValue::F32VEC2
	});

	Pipeline pipeline(vertShader, fragShader, vertBuffer);

	while (!glfwWindowShouldClose(Vulpine::Window::GetWindow())) glfwWaitEvents();

	Swapchain::Destroy();
	Instance::Destory();

	Vulpine::Window::Destroy();
}