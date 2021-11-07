#pragma once

#include "Log.h"

#define VP_ASSERT(condition, message) if(!!(condition)) VP_ERROR(message)
