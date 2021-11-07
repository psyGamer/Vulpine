#pragma once

// GLFW & Vulkan

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// GLM

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Base.h"

#define VP_ASSERT_VK(expession, message) VP_ASSERT((expession) != VK_SUCCESS, message)
