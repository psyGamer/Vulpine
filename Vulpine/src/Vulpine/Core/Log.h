#pragma once

#include "spdlog/spdlog.h"

#define VP_TRACE(...)    SPDLOG_TRACE(__VA_ARGS__)
#define VP_INFO(...)     SPDLOG_INFO(__VA_ARGS__)
#define VP_WARN(...)     SPDLOG_WARN(__VA_ARGS__)
#define VP_ERROR(...)    SPDLOG_ERROR(__VA_ARGS__)
#define VP_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
