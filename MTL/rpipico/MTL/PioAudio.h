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

#pragma once

#include "MTL/rp2040/PioClock.h"
#include "MTL/rp2040/PioI2S.h"
#include "MTL/rp2040/Dma.h"
#include "MTL/CortexM0/NVIC.h"

#define PIO_AUDIO_ATTACH_IRQ_0(pio_audio) \
     extern "C" { void IRQ_DMA_0() { pio_audio.irqHandler(); } }

#define PIO_AUDIO_ATTACH_IRQ_1(pio_audio) \
     extern "C" { void IRQ_DMA_1() { pio_audio.irqHandler(); } }

namespace MTL {

extern void PioAudio_getSamples(uint32_t* buffer, unsigned n);

//! Audio driver for Cirrus Logic CS4344/5/8 based devices
template <typename PIO_TYPE, unsigned BUFFER_SIZE = 1024, unsigned IRQ = 0>
class PioAudio
{
public:
   PioAudio(unsigned sample_freq,
            unsigned pin_mclk,
            unsigned pin_sd,
            unsigned pin_lrclk)
   {
      // Setup MCLK
      sd_clk = clk.download(pio, sample_freq * 256, pin_mclk);
      if (sd_clk < 0) return;

      // Setup I2S for SDIN, LRCLK, SCLK
      sd_i2s = i2s.download(pio, sample_freq, pin_sd, pin_lrclk);
      if (sd_i2s < 0) return;

      ch_ping = dma.allocCH();
      if (ch_ping < 0) return;

      ch_pong = dma.allocCH();
      if (ch_pong < 0) return;

      dma.CH_prog(ch_ping, ch_pong,
                  buf_ping,                 /* read_incr */  true,
                  pio.SM_getTxFIFO(sd_i2s), /* write_incr */ false,
                  BUFFER_SIZE,
                  Dma::FOUR_BYTE,
                  pio.SM_getTxDREQ(sd_i2s));

      dma.CH_prog(ch_pong, ch_ping,
                  buf_pong,                 /* read_incr */  true,
                  pio.SM_getTxFIFO(sd_i2s), /* write_incr */ false,
                  BUFFER_SIZE,
                  Dma::FOUR_BYTE,
                  pio.SM_getTxDREQ(sd_i2s));

      // Enable interrupt
      dma.CH_enableIrq(ch_ping, IRQ);
      dma.CH_enableIrq(ch_pong, IRQ);

      MTL::NVIC<11 + IRQ>().enable();
   }

   //! Start PIO
   void start()
   {
      // Prime both buffers starting with ping
      PioAudio_getSamples(buf_ping, BUFFER_SIZE);
      PioAudio_getSamples(buf_pong, BUFFER_SIZE);

      // Start MCLK and I2S
      pio.start((1 << sd_clk) | (1 << sd_i2s));

      // Start DMA ping/pong
      ping_pong = true;
      dma.CH_start(ch_ping);
   }

   //! Manually push samples
   void push(uint16_t left, uint16_t right)
   {
      pio.SM_push(sd_i2s, (left << 16) | right);
   }

   //! Handler for DMA ping/pong
   void irqHandler()
   {
      uint32_t* buffer = ping_pong ? buf_ping : buf_pong;;
      signed    ch     = ping_pong ? ch_ping  : ch_pong;

      dma.CH_clrIrq(ch, IRQ);
      dma.CH_setReadAddr(ch, buffer);

      ping_pong = not ping_pong;

      PioAudio_getSamples(buffer, BUFFER_SIZE);
   }

private:
   PIO_TYPE      pio;
   MTL::PioClock clk;
   MTL::PioI2S   i2s;
   MTL::Dma      dma;
   signed        sd_clk{-1};
   signed        sd_i2s{-1};
   signed        ch_ping{-1};
   signed        ch_pong{-1};
   bool          ping_pong {false};
   uint32_t      buf_ping[BUFFER_SIZE];
   uint32_t      buf_pong[BUFFER_SIZE];
};

} // namespace MTL
