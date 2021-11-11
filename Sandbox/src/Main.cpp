#include <string>
#include "Vulpine/Core/Window.h"

int main()
{
	Vulpine::Window::Create("Mein gude fenster", 800, 600);

	//while (!glfwWindowShouldClose(Vulpine::Window::GetWindow())) glfwWaitEvents();

	Vulpine::Window::Destroy();
}