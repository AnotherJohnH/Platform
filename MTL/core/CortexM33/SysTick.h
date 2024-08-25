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
   //! Initialize, free-running tick timer
   SysTick()
   {
      // Select the core clock, not external reference, as clock source
      reg->csr.setBit(CSR_CLKSOURCE);

      setPeriod(0x1000000);
      start();
   }

   //! Start the SysTick timer
   //
   //! \param period24 24-bit clock tick
   SysTick(unsigned period)
   {
      // Select the core clock, not external reference, as clock source
      reg->csr.setBit(CSR_CLKSOURCE);

      // Enable SysTick interrupt
      reg->csr.setBit(CSR_TICKINT);

      setPeriod(period);
      start();
   }

   //! Read current value
   operator uint32_t() const { return reg->cvr; }

   //! Check if timer is running
   bool isRunning() const { return reg->csr.getBit(CSR_ENABLE); }

   //! Check if timer had reached zero and has restarted
   bool isReset() const { return reg->csr.getBit(CSR_COUNTFLAG); }

   //! Set the tick period
   //
   //! \param ticks24 period 24-bit value
   void setPeriod(uint32_t ticks24)
   {
      reg->rvr = ticks24 - 1;
   }

   //! Start tick timer
   void start()
   {
      reg->cvr = 0;
      reg->csr.setBit(CSR_ENABLE);
   }

   //! Stop tick timer
   void stop()
   {
      reg->csr.clrBit(CSR_ENABLE);
   }

private:
   // CSR bit positions
   static const uint32_t CSR_ENABLE     = 0;
   static const uint32_t CSR_TICKINT    = 1;
   static const uint32_t CSR_CLKSOURCE  = 2;
   static const uint32_t CSR_COUNTFLAG  = 16;
};

} // namespace MTL
