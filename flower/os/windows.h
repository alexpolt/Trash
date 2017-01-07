#pragma once

#include "lib/config.h"

#define WIN32_LEAN_AND_MEAN 1

#ifdef $CONFIG_MINGW
#   undef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif

#include <windows.h>


