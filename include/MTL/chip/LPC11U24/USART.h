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

// \file USART.h
// \brief NXP LPC11U24 USART
//
// Data source NXP document "LPC11U3X-2X-1X User Manual UM10462"

#ifndef LPC11U24_USART_H
#define LPC11U24_USART_H

#include "MTL/Periph.h"

#include "IoCon.h"
#include "SysCon.h"

namespace MTL {

union USARTReg
{
   REG(0x000, rbr_thr_dll);
   REG(0x004, ier_dlm);
   REG(0x008, iir_fcr);
   REG(0x00C, lcr);
   REG(0x010, mcr);
   REG(0x014, lsr);
   REG(0x018, msr);
   REG(0x01C, scr);
   REG(0x020, acr);
   REG(0x024, icr);
   REG(0x028, fdr);
   REG(0x02C, osr);
   REG(0x030, ter);
   REG(0x040, hden);
   REG(0x048, scictrl);
   REG(0x04C, rs485ctrl);
   REG(0x050, rs485adrmatch);
   REG(0x054, rs485dly);
   REG(0x058, syncctrl);
};


class USART : public Periph<USARTReg,0x40000000>
{
public:
   void init()
   {
      IoCon  iocon;
      SysCon syscon;

      // 1. Configure Pins

      // 2. Enable USART Power and peripheral clock
      syscon.enableAHBClkCtrl(12);

      // 3. USART clock divider
      syscon.reg->uartclkdiv = 7;
   }
};


} // namespace MTL

#endif // LPC11U24_USART_H
