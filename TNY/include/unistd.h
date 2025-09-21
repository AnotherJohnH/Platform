//-------------------------------------------------------------------------------
// Copyright (C) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern int usleep(unsigned microseconds);
extern int sleep(unsigned  seconds);

#ifdef __cplusplus
}
#endif
