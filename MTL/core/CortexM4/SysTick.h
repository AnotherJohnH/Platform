//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file  SysTick.h
//! \brief Access to Cortex-M4 System Timer

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union SysTickReg
{
   REG(0x10, csr);    //!< Control and status
   REG(0x14, rvr);    //!< Reload value  24-bit
   REG(0x18, cvr);    //!< Current value 24-bit
   REG(0x1C, calib);  //!< Calibrartion  (optional)
};

class SysTick : public Periph<SysTickReg,0xE000E000>
{
public:
   SysTick() = default;

   SysTick(unsigned period)
   {
      if (period == 0)
      {
         // Try and default to 1/10th of the 10ms calib value
         period = reg->calib.getField(23, 0) / 10;

         if ((period != 0) && (reg->calib.getBit(30) == 0))
         {
            period = period + 1;
         }
      }

      if (period != 0)
      {
         setPeriod(period);

         reg->csr = CSR_CLKSOURCE | CSR_TICKINT | CSR_ENABLE;
      }
   }

   //! Check if timer is running
   bool isRunning() const { return reg->csr.getBit(CSR_ENABLE); }

   //! Set the tick period
   //
   //! \param ticks24 period 24-bit value
   void setPeriod(uint32_t ticks24)
   {
      reg->rvr = ticks24 - 1;
      reg->cvr = 0;
   }

   //! Start timer
   void start()
   {
      reg->csr.setBit(CSR_ENABLE);
   }

   //! Stop timer
   void stop()
   {
      reg->csr.clrBit(CSR_ENABLE);
   }

private:
   static const unsigned CSR_ENABLE    = 0;
   static const unsigned CSR_TICKINT   = 1;
   static const unsigned CSR_CLKSOURCE = 2;
   static const unsigned CSR_COUNTFLAG = 16;
};

} // namespace MTL

