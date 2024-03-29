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

// \brief RP2040 CLOCKS peripheral

#pragma once

#include "MTL/Periph.h"

#include "XOsc.h"
#include "Pll.h"
#include "Watchdog.h"

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
    Generator usb;
    Generator adc;
    Generator rtc;

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

class Clocks : public Periph<ClocksReg, 0x40008000>
{
public:
    enum SysFreq
    {
       SYS_FREQ_125_00_MHZ,
       SYS_FREQ_132_00_MHZ,
       SYS_FREQ_133_00_MHZ,
       SYS_FREQ_137_48_MHZ,
       SYS_FREQ_157_10_MHZ,
       SYS_FREQ_191_08_MHZ,
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
        case SYS_FREQ_132_00_MHZ: pll_sys.start(/* freq_mhz */ 132); break;
        case SYS_FREQ_133_00_MHZ: pll_sys.start(/* freq_mhz */ 133); break;
        case SYS_FREQ_137_48_MHZ: pll_sys.start137_48MHz(); break;
        case SYS_FREQ_157_10_MHZ: pll_sys.start157_10MHz(); break;
        case SYS_FREQ_191_08_MHZ: pll_sys.start191_08MHz(); break;
        }

        PllUsb pll_usb;
        pll_usb.start48MHz();

        // Reference clock
        // XOSC => CLK_REF => 12 MHz
        configGlitchless(&reg->ref, /* XOSC */ 2, /* div8 */ 0x100);

        // System clock
        // PLL_SYS => CLK_SYS => 133 MHz
        configGlitchless(&reg->sys, /* PLL SYS */ 1, /* div8 */ 0x100);

        // USB clock
        // PLL_USB => CLK_SYS => 48 MHz
        configAux(&reg->usb, /* PLL USB */ 0, /* div8 */ 0x100);

        // ADC clock
        // PLL_USB => CLK_ADC => 48 MHz
        configAux(&reg->adc, /* PLL USB */ 0, /* div8 */ 0x100);

        // RTC clock
        // XOSC => CLK_RTC => 46875 Hz
        configAux(&reg->rtc, /* XOSC */ 3,
                  /* div8 */ (uint64_t(XOsc::XTAL_FREQ_MHZ * 1000000) << 8) / RTC_FREQ);

        // PERI clock
        // PLL_SYS => CLK_PERI => 133 MHz
        configAux(&reg->peri, /* PLL SYS */ 0, /* div8 */ 0x100);

        // Start the watchdog for 1us tick (assuming 12 MHz XOsc)
        Watchdog().start(XOsc::XTAL_FREQ_MHZ);
    }

    static const unsigned RTC_FREQ = 46875;

private:
    void configGlitchless(volatile ClocksReg::Generator* clock,
                          unsigned                       source,
                          unsigned                       div8)
    {
       // Set DIV now if higher than current value
       if (div8 > clock->div)
       {
          clock->div = div8;
       }

       // Set main source
       clock->ctrl = source;

       // Wait for source selection
       while((clock->selected & (1 << source)) == 0);

       // Set DIV if not already set
       clock->div = div8;
    }

    //! Configure a clock generator with just an AUX MUX
    void configAux(volatile ClocksReg::Generator* clock,
                   unsigned                       aux_source,
                   unsigned                       div8)
    {
       // Set DIV now if higher than current value
       if (div8 > clock->div)
       {
          clock->div = div8;
       }

       // Disable clock
       clock->ctrl &= ~(1 << 11);

       // Set AUX source
       clock->ctrl = aux_source << 5;

       // Enable clock
       clock->ctrl |= 1 << 11;

       // Set DIV if not already set
       clock->div = div8;
    }
};

} // namespace MTL
