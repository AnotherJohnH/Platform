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

#pragma once

#include "MTL/chip/Pll.h"
#include "MTL/chip/Pwm.h"
#include "MTL/chip/Dma.h"
#include "MTL/chip/Irq.h"
#include "MTL/core/NVIC.h"

#define PWM_AUDIO_ATTACH_IRQ_0(pwm_audio) \
     extern "C" { void IRQ_DMA_0() { pwm_audio.irqHandler(); } }

#define PWM_AUDIO_ATTACH_IRQ_1(pio_audio) \
     extern "C" { void IRQ_DMA_1() { pwm_audio.irqHandler(); } }

namespace MTL {

extern void PwmAudio_getSamples(uint32_t* buffer, unsigned n);

//! Audio driver for PWM audio
template <unsigned PIN, unsigned BITS, unsigned BUFFER_SIZE = 1024, unsigned IRQ = 0>
class PwmAudio
{
public:
   PwmAudio(unsigned sample_freq)
   {
      unsigned divider8_4 = PllSys().getFreq() / (sample_freq * (LIMIT >> 4));

      pwm.setSysClkDiv8_4(divider8_4);

      ch_ping = dma.allocCH();
      if (ch_ping < 0) return;

      ch_reset_ping = dma.allocCH();
      if (ch_reset_ping < 0) return;

      ch_pong = dma.allocCH();
      if (ch_pong < 0) return;

      ch_reset_pong = dma.allocCH();
      if (ch_reset_pong < 0) return;

      dma.CH_prog(ch_ping, ch_reset_ping,
                  buf_ping,                 /* read_incr */  true,
                  pwm.getOut(),             /* write_incr */ false,
                  BUFFER_SIZE,
                  Dma::FOUR_BYTE,
                  pwm.getDREQ());

      dma.CH_prog(ch_reset_ping, ch_pong,
                  &buf_ping_addr,                 /* read_incr */  false,
                  dma.CH_getReadRegAddr(ch_ping), /* write_incr */ false,
                  1, Dma::FOUR_BYTE);

      dma.CH_prog(ch_pong, ch_reset_pong,
                  buf_pong,                 /* read_incr */  true,
                  pwm.getOut(),             /* write_incr */ false,
                  BUFFER_SIZE,
                  Dma::FOUR_BYTE,
                  pwm.getDREQ());

      dma.CH_prog(ch_reset_pong, ch_ping,
                  &buf_pong_addr,                 /* read_incr */  false,
                  dma.CH_getReadRegAddr(ch_pong), /* write_incr */ false,
                  1, Dma::FOUR_BYTE);

      // Enable interrupt
      dma.CH_enableIrq(ch_ping, IRQ);
      dma.CH_enableIrq(ch_pong, IRQ);

      MTL::NVIC<IRQ_DMA_0 + IRQ>().enable();
   }

   //! Start PIO
   void start()
   {
      // Prime both buffers starting with ping
      PwmAudio_getSamples(buf_ping, BUFFER_SIZE);
      PwmAudio_getSamples(buf_pong, BUFFER_SIZE);

      // Start DMA ping/pong
      dma.CH_start(ch_ping);
   }

   //! Manually push samples
   void push(int16_t left, int16_t right)
   {
      pwm = OFFSET + (left >> (16 - BITS));
   }

   //! Handler for DMA ping/pong
   void irqHandler()
   {
      if (dma.CH_isIrq(ch_ping, IRQ))
      {
         dma.CH_clrIrq(ch_ping, IRQ);
         PwmAudio_getSamples(buf_ping, BUFFER_SIZE);
      }
      else
      {
         dma.CH_clrIrq(ch_pong, IRQ);
         PwmAudio_getSamples(buf_pong, BUFFER_SIZE);
      }
   }

private:
   static const uint32_t LIMIT  = 1 << BITS;
   static const uint32_t OFFSET = LIMIT / 2;

   Pwm<PIN,/* PAIR */ true> pwm{/* clock_div_8_4 */ 0b10000, LIMIT};
   Dma                      dma{};
   signed                   ch_ping{-1};
   signed                   ch_reset_ping{-1};
   signed                   ch_pong{-1};
   signed                   ch_reset_pong{-1};
   uint32_t                 buf_ping[BUFFER_SIZE];
   uint32_t                 buf_pong[BUFFER_SIZE];
   uint32_t                 buf_ping_addr{uint32_t(&buf_ping)};
   uint32_t                 buf_pong_addr{uint32_t(&buf_pong)};
};

} // namespace MTL
