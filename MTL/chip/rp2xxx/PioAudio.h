//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/Pins.h"
#include "MTL/chip/PioClock.h"
#include "MTL/chip/PioI2S.h"

#include "MTL/chip/rp2xxx/Audio.h"

namespace MTL {

//! Audio driver for Cirrus Logic CS4344/5/8 based devices
template <typename PIO_TYPE, unsigned BUFFER_SIZE = 1024, unsigned IRQ = 0>
class PioAudio : public Audio::Base<BUFFER_SIZE,IRQ>
{
public:
   PioAudio(unsigned       sample_freq,
            unsigned       pin_sd,
            unsigned       pin_lrclk_sclk,
            unsigned       pin_mclk           = PIN_IGNORE,
            Audio::Format  format             = Audio::STEREO_16,
            bool           lsb_lrclk_msb_sclk = true)
       : i2s(format, lsb_lrclk_msb_sclk)
   {
      if (pin_mclk != PIN_IGNORE)
      {
          // Setup MCLK
          sd_clk = clk.download(sample_freq * 256, pin_mclk);
          if (sd_clk < 0) return;
      }

      // Setup I2S for SDIN, LRCLK, SCLK
      sd_i2s = i2s.download(pio, sample_freq, pin_sd, pin_lrclk_sclk);
      if (sd_i2s < 0) return;
   }

   void start()
   {
      this->startDMA(pio.SM_getTxDREQ(sd_i2s), pio.SM_getTxFIFO(sd_i2s));

      if (sd_clk >= 0)
      {
         // Start MCLK and I2S
         pio.start((1 << sd_clk) | (1 << sd_i2s));
      }
      else
      {
         // Start I2S
         pio.start(1 << sd_i2s);
      }
   }

   //! Manually push samples
   void push(uint16_t left, uint16_t right)
   {
      pio.SM_push(sd_i2s, (left << 16) | right);
   }

private:
   PIO_TYPE                pio;
   MTL::PioClock<PIO_TYPE> clk;
   MTL::PioI2S             i2s;
   signed                  sd_clk{-1};
   signed                  sd_i2s{-1};
};

} // namespace MTL
