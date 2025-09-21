//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

static FILE handles[4];

FILE* stdin  = &handles[0];
FILE* stdout = &handles[1];
FILE* stderr = &handles[2];
