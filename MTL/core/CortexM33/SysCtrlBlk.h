//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Access Cortex-M33 System Control Block

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
