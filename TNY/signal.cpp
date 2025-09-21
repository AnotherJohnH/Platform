//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <signal.h>

#include "MTL/MTL.h"

static signal_handler_t signals[NUM_SIGNALS] =
{
   SIG_DFL,
   SIG_DFL,
   SIG_DFL,
   SIG_DFL,
   SIG_DFL,
   SIG_DFL
};

signal_handler_t signal(int signal, signal_handler_t func)
{
}

int raise(int signal)
{
   if ((signal < 0) || (signal >= NUM_SIGNALS))
      return -1;

   return 0;
}
