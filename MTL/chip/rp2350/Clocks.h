//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// \brief RP2350 CLOCKS peripheral

#pragma once

#include "MTL/Periph.h"

#include "XOsc.h"
#include "Pll.h"

namespace MTL {

struct ClocksReg
{
    struct Generator
    {
       uint32_t ctrl;
       uint32_t div;
       uint32_t selected;
    };

    Generator gpio[4];
    Generator ref;
    Generator sys;
    Generator peri;
    Generator hstx;
    Generator usb;
    Generator adc;

    uint32_t dftclk_xosc_ctrl;
    uint32_t dftclk_rosc_ctrl;
    uint32_t dftclk_lposc_ctrl;
    uint32_t clk_sys_resus_ctrl;
    uint32_t clk_sys_resus_status;
    uint32_t fc0_ref_khz;
    uint32_t fc0_min_khz;
    uint32_t fc0_max_khz;
    uint32_t fc0_delay;
    uint32_t fc0_interval;
    uint32_t fc0_src;
    uint32_t fc0_status;
    uint32_t fc0_result;
    uint32_t wake_en0;
    uint32_t wake_en1;
    uint32_t sleep_en0;
    uint32_t sleep_en1;
    uint32_t enabled0;
    uint32_t enabled1;
    uint32_t intr;
    uint32_t inte;
    uint32_t intf;
    uint32_t ints;
};

class Clocks : public Periph<ClocksReg, 0x40010000>
{
public:
    enum SysFreq
    {
       SYS_FREQ_125_00_MHZ,
       SYS_FREQ_133_00_MHZ,
       SYS_FREQ_150_00_MHZ,
       SYS_FREQ_191_08_MHZ
    };

    void start()
    {
        // Disable resus
        reg->clk_sys_resus_ctrl = 0;

        // Start the XTAL oscillator
        XOsc().start();

        // Before starting PLLs ensure SYS and REF clocks switched away from aux sources
        reg->sys.ctrl &= ~0b1;
        while(reg->sys.selected != 1);

        reg->ref.ctrl &= ~0b11;
        while(reg->ref.selected != 1);

        // Start the PLLs
        PllSys pll_sys;
        extern SysFreq clocks_sys_freq;
        switch(clocks_sys_freq)
        {
        case SYS_FREQ_125_00_MHZ: pll_sys.start(/* freq_mhz */ 125); break;
        case SYS_FREQ_133_00_MHZ: pll_sys.start(/* freq_mhz */ 133); break;
        case SYS_FREQ_150_00_MHZ: pll_sys.start150MHz(); break;
        case SYS_FREQ_191_08_MHZ: pll_sys.start191_08MHz(); break;
        }

        PllUsb pll_usb;
        pll_usb.start48MHz();

        // Reference clock
        // XOSC => CLK_REF => 12 MHz
        configGlitchless(&reg->ref, /* XOSC */ 2, /* div16 */ 0x10000);

        // System clock
        // PLL_SYS => CLK_SYS => 150 MHz
        configGlitchless(&reg->sys, /* PLL SYS */ 1, /* div16 */ 0x10000);

        // PERI clock
        // PLL_SYS => CLK_PERI => 150 MHz
        configAux(&reg->peri, /* PLL SYS */ 0, /* div16 */ 0x10000);

        // USB clock
        // PLL_USB => CLK_SYS => 48 MHz
        configAux(&reg->usb, /* PLL USB */ 0, /* div16 */ 0x10000);

        // ADC clock
        // PLL_USB => CLK_ADC => 48 MHz
        configAux(&reg->adc, /* PLL USB */ 0, /* div16 */ 0x10000);

        // HSTX clock
        // PLL_SYS => CLK_HSTX => 150 MHz
        configAux(&reg->hstx, /* CLK SYS */ 0, /* div16 */ 0x10000);
    }

private:
    void configGlitchless(volatile ClocksReg::Generator* clock,
                          unsigned                       source,
                          unsigned                       div16)
    {
       // Set DIV now if higher than current value
       if (div16 > clock->div)
       {
          clock->div = div16;
       }

       // Set main source
       clock->ctrl = source;

       // Wait for source selection
       while((clock->selected & (1 << source)) == 0);

       // Set DIV if not already set
       clock->div = div16;
    }

    //! Configure a clock generator with just an AUX MUX
    void configAux(volatile ClocksReg::Generator* clock,
                   unsigned                       aux_source,
                   unsigned                       div16)
    {
       // Set DIV now if higher than current value
       if (div16 > clock->div)
       {
          clock->div = div16;
       }

       // Disable clock
       clock->ctrl &= ~(1 << 11);

       // Set AUX source
       clock->ctrl = aux_source << 5;

       // Enable clock
       clock->ctrl |= 1 << 11;

       // Set DIV if not already set
       clock->div = div16;
    }
};

} // namespace MTL
