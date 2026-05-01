//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <stdarg.h>
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int snprintf(char*, size_t, const char*, ...);
extern int sprintf(char*, const char*, ...);
extern int vsnprintf(char*, size_t, const char*, va_list);
extern int vsprintf(char*, const char*, va_list);

#ifdef __cplusplus
}
#endif
