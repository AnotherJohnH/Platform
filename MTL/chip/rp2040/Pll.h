//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief RP2040 PLL_SYS peripheral

#pragma once

#include "MTL/Periph.h"

#include "Resets.h"

namespace MTL {

struct PllReg
{
   uint32_t cs;
   uint32_t pwr;
   uint32_t fbdiv_int;
   uint32_t prim;
};

template <uint32_t BASE_ADDRESS, unsigned RESET_BIT>
class PllBase : public Periph<PllReg, BASE_ADDRESS>
{
public:
   //! Return configured frequency of PLL (Hz)
   unsigned getFreq() const { return freq; }

   //! Start PLL at 133 MHz (assumes a 12 MHz XTAL input)
   void start133MHz()
   {
      // XTAL          REF      FBDIV    VCO                 POSTDIV
      // 12 MHz / 1 => 12 MHz * 133   => 1596 MHz / 6 / 2 => 133 MHz
      config(/* refdiv */ 1, /* fbdiv */ 133, /* post_div1 */ 6, /* post_div2 */ 2);

      freq = 133000000;
   }

   //! Start PLL at 125 MHz (assumes a 12 MHz XTAL input)
   void start125MHz()
   {
      // XTAL          REF      FBDIV    VCO                 POSTDIV
      // 12 MHz / 1 => 12 MHz * 125   => 1500 MHz / 6 / 2 => 125 MHz
      config(/* refdiv */ 1, /* fbdiv */ 125, /* post_div1 */ 6, /* post_div2 */ 2);

      freq = 125000000;
   }

   //! Start PLL at 48 MHz (assumes a 12 MHz XTAL input)
   void start48MHz()
   {
      // XTAL          REF      FBDIV    VCO                 POSTDIV
      // 12 MHz / 1 => 12 MHz * 100   => 1200 MHz / 5 / 5 => 48 MHz
      config(/* refdiv */ 1, /* fbdiv */ 100, /* post_div1 */ 5, /* post_div2 */ 5);

      freq = 48000000;
   }

private:
   void config(unsigned refdiv,
               unsigned fbdiv,
               unsigned post_div1,
               unsigned post_div2)
   {
       // Reset the PLL
       Resets().tglReset(RESET_BIT);

       // Program ref and feedback dividers
       this->reg->cs        = refdiv;
       this->reg->fbdiv_int = fbdiv;

       // Turn on PLL          PWR.VCOPD=0 PWR.PD=0
       this->reg->pwr &= ~((1<<5) | (1<<0));

       // Wait for PLL lock    CS.LOCK == 1
       while((this->reg->cs & (1<<31)) == 0);

       // Setup post dividers
       this->reg->prim = (post_div1 << 16) | (post_div2 << 12);

       // Turn on post divider PWR.POSTDIVPD=0
       this->reg->pwr &= ~(1<<3);
   }

   unsigned freq {};
};

class PllSys : public PllBase<0x40028000, Resets::PLL_SYS> {};

class PllUsb : public PllBase<0x4002C000, Resets::PLL_USB> {};

} // namespace MTL
