//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

//! \brief Access Cortex-M0 System Control Block

#pragma once

#include "MTL/Periph.h"

namespace MTL {

//! Cortex-M0 system timer registers
union SysCtrlBlkReg
{
   REG(0x00, cpuid);   //!< CPUID Register
   REG(0x04, icsr);    //!< Interrupt Control and State Register
   REG(0x0C, aircr);   //!< Application Interrupt Reset Control Register
   REG(0x10, scr);     //!< System Control Register
   REG(0x14, ccr);     //!< Configuration and Control Register
   REG(0x1C, shpr2);   //!< System Handler Priority Register 2
   REG(0x20, shpr3);   //!< System handler Priority Register 3
};

//! Access Cortex-M0 system timer
class SysCtrlBlk : public Periph<SysCtrlBlkReg,0xE000ED00>
{
public:
   bool isSysTickPend() { return reg->icsr.getBit(26); }
   void pendSysTick() { reg->icsr.setBit(26); }
   void clearSysTick() { reg->icsr.setBit(25); }
};

} // namespace MTL
