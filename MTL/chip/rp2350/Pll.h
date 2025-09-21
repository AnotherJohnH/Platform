//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2350 PLL_SYS and PLL_USB peripherals

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
   uint32_t intr;
   uint32_t inte;
   uint32_t intf;
   uint32_t ints;
};

template <uint32_t BASE_ADDRESS, unsigned RESET_BIT>
class Pll : public Periph<PllReg, BASE_ADDRESS>
{
public:
   //! Return configured frequency of PLL (Hz)
   unsigned getFreq() const { return freq; }

   //! Start PLL (assumes a 12 MHz XTAL input)
   void start(unsigned freq_mhz)
   {
      // XTAL          REF      FBDIV    VCO                   POSTDIV
      // 12 MHz / 1 => 12 MHz * freq  => (f*12) MHz / 6 / 2 => freq MHz
      config(/* refdiv */ 1, /* fbdiv */ freq_mhz, /* post_div1 */ 6, /* post_div2 */ 2);

      freq = freq_mhz * 1000000;
   }

   //! Start PLL at 48 MHz (assumes a 12 MHz XTAL input)
   void start48MHz()
   {
      // XTAL          REF      FBDIV    VCO                 POSTDIV
      // 12 MHz / 1 => 12 MHz * 100   => 1200 MHz / 5 / 5 => 48 MHz
      config(/* refdiv */ 1, /* fbdiv */ 100, /* post_div1 */ 5, /* post_div2 */ 5);

      freq = 48000000;
   }

   //! Start PLL at 150 MHz (assumes a 12 MHz XTAL input)
   void start150MHz()
   {
      // XTAL          REF      FBDIV    VCO                 POSTDIV
      // 12 MHz / 1 => 12 MHz * 125 => 1500 MHz / 1 / 7 => 150 MHz
      config(/* refdiv */ 1, /* fbdiv */ 125, /* post_div1 */ 5, /* post_div2 */ 2);

      freq = 150000000;
   }

   //! Start PLL at 191.08 MHz (assumes a 12 MHz XTAL input)
   void start191_08MHz()
   {
      // XTAL          REF      FBDIV    VCO                 POSTDIV
      // 12 MHz / 28 => 429 KHz * 3121 => 1337.6 MHz / 1 / 7 => 191.08 MHz
      config(/* refdiv */ 28, /* fbdiv */ 3121, /* post_div1 */ 7, /* post_div2 */ 1);

      freq = 191081633;
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

   static unsigned freq;
};

template <uint32_t BASE_ADDRESS, unsigned RESET_BIT>
unsigned MTL::Pll<BASE_ADDRESS, RESET_BIT>::freq {0};

using PllSys = Pll<0x40050000, Resets::PLL_SYS>;
using PllUsb = Pll<0x40058000, Resets::PLL_USB>;

} // namespace MTL
