//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040 IO_BANK peripheral

#pragma once

#include "MTL/Periph.h"
#include "PadsBank.h"

namespace MTL {

struct IoBankReg
{
   struct Gpio
   {
       uint32_t status;
       uint32_t ctrl;
   };

   Gpio     gpio[30];
   uint32_t intr[4];
   uint32_t proc0_inte[4];
   uint32_t proc0_intf[4];
   uint32_t proc0_ints[4];
   uint32_t proc1_inte[4];
   uint32_t proc1_intf[4];
   uint32_t proc1_ints[4];
   uint32_t dormant_wake_inte[4];
   uint32_t dormant_wake_intf[4];
   uint32_t dormant_wake_ints[4];
};

class IoBank : public Periph<IoBankReg, 0x40014000>
{
public:
   enum Func
   {
       XIP   = 0,
       SPI   = 1,
       UART  = 2,
       I2C   = 3,
       PWM   = 4,
       SIO   = 5,
       PIO0  = 6,
       PIO1  = 7,
       CLOCK = 8,
       USB   = 9
   };

   //! Set the function for an I/O pin
   void setFunc(unsigned io_pin, Func func, uint8_t pad_ctrl = 0)
   {
      PadsBank pads;

      pads.set(io_pin, PadsBank::IE | pad_ctrl);
      reg->gpio[io_pin].ctrl = func;
   }
};

} // namespace MTL
