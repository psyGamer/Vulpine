#pragma once

#include <string>

#include "Log.h"

#define VP_ASSERT(condition, message) if(!!(condition)) VP_ERROR(std::string("Assertion failed: ") + std::string(message))
