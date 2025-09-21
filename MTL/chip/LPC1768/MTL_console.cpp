//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "UART.h"

static MTL::UART0 uart(MTL::UART::BAUD_9600);

void MTL_putch(uint8_t ch)
{
   uart.tx(ch);
}

int MTL_getch()
{
   return uart.rx();
}
