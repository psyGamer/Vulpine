#pragma once

#pragma warning(disable : 26812) // Vulkan issue

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VP_ASSERT_VK(expession, message) VP_ASSERT((expession) != VK_SUCCESS, message)
