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
//! \brief Access to Cortex-M0 System Timer

// NOTE: This private peripheral is optional


#ifndef CORTEX_M0_SYS_TIMER_H
#define CORTEX_M0_SYS_TIMER_H


#include "MTL/Periph.h"


#define SYS_TIMER_ATTACH_IRQ(OBJ) \
     extern "C" { void SysTimer_IRQ() { OBJ.tick(); } }


namespace MTL {

union SysTimerReg
{
   REG(0x10, csr);
   REG(0x14, rvr);
   REG(0x18, cvr);
   REG(0x1C, calib);
};


class SysTimer : public Periph<SysTimerReg,0xE000E000>
{
private:
   static const uint32_t CSR_ENABLE     = 1<<0;
   static const uint32_t CSR_EXCEPTION  = 1<<1;
   static const uint32_t CSR_CORE_CLOCK = 1<<2;

   volatile uint32_t  ticks{0};

public:
   SysTimer()
   {
      setPeriod(getPeriod10ms());

      reg->csr |= CSR_EXCEPTION;
   }

   uint32_t getPeriod10ms() const
   {
      uint32_t period = reg->calib & 0xFFFFFF;

      if ((period != 0) && ((reg->calib & (1<<30)) == 0))
      {
         return period + 1;
      }

      reg->csr |= CSR_CORE_CLOCK;

      return CORE_CLOCK_MHZ * 10000;
   }

   //! Check if timer is runnign
   bool isRunning() const { return (reg->csr & CSR_ENABLE) != 0; }

   //! Get the current tick value
   operator uint32_t() const { return ticks; }

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
      reg->csr |= CSR_ENABLE;
   }

   //! Stop timer
   void stop()
   {
      reg->csr &= ~CSR_ENABLE;
   }

   //! Increment the current tick count
   //
   //! This method is called from the SysTimer interrupt vector
   void tick()
   {
      ++ticks;
   }
};


} // namespace MTL

#endif // CORTEX_M0_SYS_TIMER_H
