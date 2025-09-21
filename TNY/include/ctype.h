//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int isdigit(int c);
extern int isalpha(int c);
extern int islower(int c);
extern int isupper(int c);
extern int isalnum(int c);
extern int isspace(int c);

#ifdef __cplusplus
}
#endif
