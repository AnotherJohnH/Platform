//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "Clocks.h"
#include "Resets.h"
#include "Ticks.h"

//! Default system clock freqency 150 MHz
//  declared weak so that applications may override
namespace MTL {
Clocks::SysFreq __attribute__((weak)) clocks_sys_freq = Clocks::SYS_FREQ_150_00_MHZ;
}

void MTL_init()
{
   // Resets for essential peripherals
   const uint32_t ESSENTIAL_RESETS = MTL::Resets::USBCTRL   |
                                     MTL::Resets::PLL_USB   |
                                     MTL::Resets::PLL_SYS   |
                                     MTL::Resets::PADS_QSPI |
                                     MTL::Resets::IO_QSPI   |
                                     MTL::Resets::SYSCFG;

   const uint32_t CLOCKED_RESETS = MTL::Resets::USBCTRL |
                                   MTL::Resets::UART1   |
                                   MTL::Resets::UART0   |
                                   MTL::Resets::SPI1    |
                                   MTL::Resets::SPI0    |
                                   MTL::Resets::ADC     |
                                   MTL::Resets::HSTX;

   MTL::Resets resets;

   // Hold in reset everything except essentials
   resets.setReset(MTL::Resets::ALL & ~ESSENTIAL_RESETS);

   // Clear reset for all except some clocked peripherals
   resets.clrReset(MTL::Resets::ALL & ~CLOCKED_RESETS);

   // Start XOSC, PLLs and all the clock generators
   MTL::Clocks().start();

   // Start all the tick generators
   MTL::Ticks().start();

   // Clear resets for everything
   resets.clrReset(MTL::Resets::ALL);
}
