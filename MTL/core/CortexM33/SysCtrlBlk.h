//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Access Cortex-M33 System Control Block

#pragma once

#include "MTL/Periph.h"

namespace MTL {

//! Cortex-M33 system timer registers
union SysCtrlBlkReg
{
   REG(0x00, cpuid);   //!< CPUID 
   REG(0x04, icsr);    //!< Interrupt Control and State 
   REG(0x08, vtor);    //!< Vector Table Offset
   REG(0x0C, aircr);   //!< Application Interrupt Reset Control
   REG(0x10, scr);     //!< System Control
   REG(0x14, ccr);     //!< Configuration and Control
   REG(0x1C, shpr2);   //!< System Handler Priority 2
   REG(0x20, shpr3);   //!< System handler Priority 3
   REG(0x88, cpacr);   //!< Coprocessor Access Control
};

//! Access Cortex-M0 system timer
class SysCtrlBlk : public Periph<SysCtrlBlkReg,0xE000ED00>
{
public:
   bool isSysTickPend() { return reg->icsr.getBit(26); }
   void pendSysTick() { reg->icsr.setBit(26); }
   void clearSysTick() { reg->icsr.setBit(25); }

   void enableFP()
   {
       // Set full access in CP11 and CP10
       reg->cpacr = reg->cpacr | (0b11 << 22) | (0b11 << 20);

       asm("dsb");
       asm("isb");
   }
};

} // namespace MTL
