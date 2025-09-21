//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdlib.h>

#include "MTL/MTL.h"

void exit(int status)
{
   MTL_halt(status);
}
