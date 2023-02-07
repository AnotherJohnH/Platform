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

//! \file SysTimer.h
//! \brief Access to Cortex-M3 System Timer

#ifndef CORTEX_M3_SYS_TIMER_H
#define CORTEX_M3_SYS_TIMER_H

#include "MTL/Periph.h"


namespace MTL {

union SysTimerReg
{
   REG(0x10, csr);    //!< Control and status
   REG(0x14, rvr);    //!< Reload value
   REG(0x18, cvr);    //!< Current value
   REG(0x1C, calib);  //!< Calibrartion
};


class SysTimer : public Periph<SysTimerReg,0xE000E000>
{
private:
   static const uint32_t CSR_ENABLE     = 0;
   static const uint32_t CSR_TICKINT    = 1;
   static const uint32_t CSR_CLKSOURCE  = 2;
   static const uint32_t CSR_COUNTFLAG  = 16;

public:
   SysTimer(unsigned period = 0)
   {
      if (period == 0)
      {
         period = reg->calib.getField(23, 0) / 10;

         if ((period != 0) && (reg->calib.getBit(30) == 0))
         {
            period = period + 1;
         }
      }

      if (period != 0)
      {
         setPeriod(period);

         reg->csr.setBit(CSR_CLKSOURCE);
         reg->csr.setBit(CSR_TICKINT);

         start();
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
};


} // namespace MTL

#endif // CORTEX_M3_SYS_TIMER_H
