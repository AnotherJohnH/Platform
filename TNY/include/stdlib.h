//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#include "UCL/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void* malloc(size_t);
extern void  free(void*);
extern int   atexit(void (*)(void));
extern void  exit(int);
extern void  abort(void);
extern int   system(const char*);

#ifdef __cplusplus
}
#endif
