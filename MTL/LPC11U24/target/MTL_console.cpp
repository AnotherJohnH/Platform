//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "MTL/LPC11U24/USART.h"

static MTL::USART uart(MTL::USART::BAUD_9600);

void MTL_putch(uint8_t ch)
{
   uart.tx(ch);
}

int MTL_getch()
{
   return uart.rx();
}
