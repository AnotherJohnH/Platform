//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "Clocks.h"
#include "Resets.h"

void MTL_init()
{
   // Resets for essential peripherals
   const uint32_t ESSENTIAL_RESETS = MTL::Resets::IO_QSPI   |
                                     MTL::Resets::PADS_QSPI |
                                     MTL::Resets::PLL_USB   |
                                     MTL::Resets::USBCTRL   |
                                     MTL::Resets::SYSCFG    |
                                     MTL::Resets::PLL_SYS;

   const uint32_t CLOCKED_RESETS = MTL::Resets::USBCTRL |
                                   MTL::Resets::UART1   |
                                   MTL::Resets::UART0   |
                                   MTL::Resets::SPI1    |
                                   MTL::Resets::SPI0    |
                                   MTL::Resets::RTC     |
                                   MTL::Resets::ADC;

   MTL::Resets resets;

   // reset everything except essentials
   resets.setReset(MTL::Resets::ALL & ~ESSENTIAL_RESETS);

   // Clear reset for all except some clocked peropherals
   resets.clrReset(MTL::Resets::ALL & ~CLOCKED_RESETS);

   // Start XOSC, PLLs and all the clock generators
   MTL::Clocks().start();

   // Clear resets for everything
   resets.clrReset(MTL::Resets::ALL);
}
