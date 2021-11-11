#include <string>
#include "Vulpine/Core/Window.h"

// Use windowed app if it's in the Dist configuration
#ifdef VP_DIST
	#define MAIN WinMain
#else
	#define MAIN main
#endif

void MAIN()
{
	Vulpine::Window::Create("Mein gude fenster", 800, 600);

	while (!glfwWindowShouldClose(Vulpine::Window::GetWindow())) glfwWaitEvents();

	Vulpine::Window::Destroy();
}