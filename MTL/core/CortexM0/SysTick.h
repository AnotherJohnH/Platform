//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Access Cortex-M0 system timer

// XXX: This private peripheral is optional

#pragma once

#include "MTL/Periph.h"

namespace MTL {

//! Cortex-M0 system timer registers
union SysTickReg
{
   REG(0x10, csr);   //!< Control and status
   REG(0x14, rvr);   //!< Reload value
   REG(0x18, cvr);   //!< Current value
   REG(0x1C, calib); //!< Calibrartion (XXX optional)
};

class SysTick : public Periph<SysTickReg,0xE000E000>
{
public:
   //! Start the SysTick timer
   //! \param period24 24-bit clock tick
   SysTick(unsigned period = 0x1000000)
   {
      setPeriod(period);
      start();

      __asm volatile("cpsie i");
   }

   //! Read current value
   operator uint32_t() const { return reg->cvr; }

   //! Check if timer is running
   bool isRunning() const { return reg->csr.getBit(CSR_ENABLE); }

   //! Check if timer had reached zero and has restarted
   bool isReset() const { return reg->csr.getBit(CSR_COUNTFLAG); }

   //! Set the tick period
   //! \param ticks24 period 24-bit value
   void setPeriod(uint32_t ticks24)
   {
      reg->rvr = ticks24 - 1;
   }

   //! Start tick timer
   void start()
   {
      reg->cvr = 0;

      // Select the core clock, not external reference, as clock source
      reg->csr = (1 << CSR_CLKSOURCE) |
                 (1 << CSR_TICKINT) |
                 (1 << CSR_ENABLE);
   }

   //! Stop tick timer
   void stop()
   {
      reg->csr = 0;
   }

private:
   // CSR bit positions
   static const uint32_t CSR_ENABLE     = 0;
   static const uint32_t CSR_TICKINT    = 1;
   static const uint32_t CSR_CLKSOURCE  = 2;
   static const uint32_t CSR_COUNTFLAG  = 16;
};

} // namespace MTL
