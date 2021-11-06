#pragma once

#include "spdlog/spdlog.h"

#define VP_TRACE(message, ...)		spdlog::trace(message, __VA_ARGS__)
#define VP_INFO(message, ...)		spdlog::info(message, __VA_ARGS__)
#define VP_WARN(message, ...)		spdlog::warn(message, __VA_ARGS__)
#define VP_ERROR(message, ...)		spdlog::error(message, __VA_ARGS__)
#define VP_CRITICAL(message, ...)	spdlog::critical(message, __VA_ARGS__)
