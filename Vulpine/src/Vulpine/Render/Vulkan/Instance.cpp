#include "vppch.h"
#include "Instance.h"

#include "Device.h"

namespace Vulpine::Vulkan
{
	void Instance::Create()
	{
		// Application Info
	
		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		
		applicationInfo.pApplicationName = "Vulpine Application";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Vulpine Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);

		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		// Instance Info

		const std::vector<const char*> instanceLayers = {
#ifdef _DEBUG
		"VK_LAYER_KHRONOS_validation"
#endif
		};

		uint32_t glfwExtensionCount;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		instanceInfo.pApplicationInfo = &applicationInfo;

		instanceInfo.enabledLayerCount = instanceLayers.size();
		instanceInfo.ppEnabledLayerNames = instanceLayers.data();
		instanceInfo.enabledExtensionCount = glfwExtensionCount;
		instanceInfo.ppEnabledExtensionNames = glfwExtensions;

		VP_ASSERT_VK(vkCreateInstance(&instanceInfo, nullptr, &s_Instance), "Failed to create instance");


		Device::Create(s_Instance, nullptr); // TODO Get surface from GLFW
	}

	void Instance::Destory()
	{
		vkDestroyInstance(s_Instance, nullptr);
	}
}