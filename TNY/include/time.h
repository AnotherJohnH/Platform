//-------------------------------------------------------------------------------
// Copyright (C) 2013-2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#include <stdlib.h>

#define  CLOCKS_PER_SEC 100

typedef signed time_t;
typedef signed clock_t;

#ifdef __cplusplus
extern "C" {
#endif

extern time_t  time(time_t*);
extern clock_t clock(void);

#ifdef __cplusplus
}
#endif
