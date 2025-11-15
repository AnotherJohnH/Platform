//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
//   MTL::AudioFormat specifes the sample buffer format
//
//   STEREO_16         32 bit word              One 32bit word for one sample pair
//                     +---------+---------+
//                     | RIGHT   | LEFT    |
//                     +---------+---------+
//
//   MONO_16           32 bit word              One 32bit word for two samples
//                     +---------+---------+    Same 16-bit MONO value sent to left
//                     | MONO-2  | MONO-1  |    and right channels of stereo devices
//                     +---------+---------+
//
//   STERO_PAIRS_16    32 bit word              Two 32bit words for two sample pairs
//                     +---------+---------+
//                     | LEFT-2  | LEFT-1  |
//                     +---------+---------+
//                     | RIGHT-2 | RIGHT-1 |
//                     +---------+---------+

#pragma once

#include "MTL/chip/Dma.h"
#include "MTL/chip/Irq.h"
#include "MTL/core/NVIC.h"

#define MTL_AUDIO_ATTACH_IRQ_0(audio) \
     extern "C" { void IRQ_DMA_0() { audio.irqHandler(); } }

#define MTL_AUDIO_ATTACH_IRQ_1(audio) \
     extern "C" { void IRQ_DMA_1() { audio.irqHandler(); } }

namespace MTL {

namespace Audio {

enum Format
{
   MONO_16,         //!<  Single 16-bit sample may be copied to L and R in stereo devices
   STEREO_16,       //!<  Alternating L/R 16-bit samples
   STEREO_PAIRS_16  //!<  Alternating L/R pairs of 16-bit samples
};

extern void getSamples(uint32_t* buffer, unsigned n);

//! Base class for audio drivers
template <unsigned BUFFER_SIZE, unsigned IRQ>
class Base
{
public:
   Base()
   {
      ch_ping = dma.allocCH();
      if (ch_ping < 0) return;

      ch_reset_ping = dma.allocCH();
      if (ch_reset_ping < 0) return;

      ch_pong = dma.allocCH();
      if (ch_pong < 0) return;

      ch_reset_pong = dma.allocCH();
      if (ch_reset_pong < 0) return;

      // Enable interrupt
      dma.CH_enableIrq(ch_ping, IRQ);
      dma.CH_enableIrq(ch_pong, IRQ);

      MTL::NVIC<IRQ_DMA_0 + IRQ>().enable();
   }

   //! Handler for DMA ping/pong
   void irqHandler()
   {
      if (dma.CH_isIrq(ch_ping, IRQ))
      {
         dma.CH_clrIrq(ch_ping, IRQ);
         getSamples(buf_ping, BUFFER_SIZE);
      }
      else
      {
         dma.CH_clrIrq(ch_pong, IRQ);
         getSamples(buf_pong, BUFFER_SIZE);
      }
   }

protected:
   //! Start audio streaming
   void startDMA(uint32_t dreq_, volatile uint32_t* out_reg_)
   {
      // Prime both buffers starting with ping
      getSamples(buf_ping, BUFFER_SIZE);
      getSamples(buf_pong, BUFFER_SIZE);

      // Program a loop of DMA channels
      dma.CH_prog(ch_ping, ch_reset_ping,
                  buf_ping,                 /* read_incr */  true,
                  out_reg_,                 /* write_incr */ false,
                  BUFFER_SIZE,
                  Dma::FOUR_BYTE,
                  dreq_);

      dma.CH_prog(ch_reset_ping, ch_pong,
                  &buf_ping_addr,                 /* read_incr */  false,
                  dma.CH_getReadRegAddr(ch_ping), /* write_incr */ false,
                  1, Dma::FOUR_BYTE);

      dma.CH_prog(ch_pong, ch_reset_pong,
                  buf_pong,                 /* read_incr */  true,
                  out_reg_,                 /* write_incr */ false,
                  BUFFER_SIZE,
                  Dma::FOUR_BYTE,
                  dreq_);

      dma.CH_prog(ch_reset_pong, ch_ping,
                  &buf_pong_addr,                 /* read_incr */  false,
                  dma.CH_getReadRegAddr(ch_pong), /* write_incr */ false,
                  1, Dma::FOUR_BYTE);

      // Start DMA ping/pong
      dma.CH_start(ch_ping);
   }

private:
   Dma      dma{};
   signed   ch_ping{-1};
   signed   ch_reset_ping{-1};
   signed   ch_pong{-1};
   signed   ch_reset_pong{-1};
   uint32_t buf_ping[BUFFER_SIZE];
   uint32_t buf_pong[BUFFER_SIZE];
   uint32_t buf_ping_addr{uint32_t(&buf_ping)};
   uint32_t buf_pong_addr{uint32_t(&buf_pong)};
};

} // namespace Audio

} // namespace MTL
