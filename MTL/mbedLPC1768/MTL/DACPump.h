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

#include "MTL/MTL.h"

#include "MTL/LPC1768/DAC.h"
#include "MTL/LPC1768/GPDMA.h"

#include "MTL/CortexM3/NVIC.h"

#define DAC_PUMP_ATTACH_IRQ(pump) \
     extern "C" { void IRQ_GPDMA() { pump.irqHandler(); } }

namespace MTL {

extern void DACPump_getSamples(uint32_t* buffer, unsigned n);

template <unsigned BUFFER_SIZE = 1024>
class DACPump
{
public:
   DACPump(unsigned sample_freq_, unsigned dma_channel_ = 0)
      : dac (sample_freq_, /* DMA */ true)
      , dma_channel(dma_channel_)
   {
      // Enable GPDMA interrupts
      MTL::NVIC<26>().enable();

      // Configure GPDMA for memory to DAC DMA
      gpdma.configChan(dma_channel,
                       MTL::GPDMA::MEMORY, MTL::GPDMA::DAC, /* irq */ true);

      // Setup pair of DMA transfers in an infinite loop
      dma_ping.setSrc(ping);
      dma_ping.setDest(&dac.reg->dacr);
      dma_ping.setNext(&dma_pong);
      dma_ping.setControl(MTL::DMA::BURST16, MTL::DMA::WIDTH32, /* src_inc */ true,
                      MTL::DMA::BURST1,  MTL::DMA::WIDTH32, /* dst_inc */ false,
                      BUFFER_SIZE, /* irq */ true);

      dma_pong.setSrc(pong);
      dma_pong.setDest(&dac.reg->dacr);
      dma_pong.setNext(&dma_ping);
      dma_pong.setControl(MTL::DMA::BURST16, MTL::DMA::WIDTH32, /* src_inc */ true,
                      MTL::DMA::BURST1,  MTL::DMA::WIDTH32, /* dst_inc */ false,
                      BUFFER_SIZE, /* irq */ true);
   }

   void start()
   {
      // Prime both buffers starting with ping
      DACPump_getSamples(ping, BUFFER_SIZE);
      DACPump_getSamples(pong, BUFFER_SIZE);

      // Start DMA with ping buffer
      ping_pong = true;

      volatile MTL::DMA* dma = gpdma.getHead(dma_channel);
      dma->setSrc(ping);
      dma->setDest(&dac.reg->dacr);
      dma->setNext(&dma_pong);
      dma->setControl(MTL::DMA::BURST16, MTL::DMA::WIDTH32, /* src_inc */ true,
                      MTL::DMA::BURST1,  MTL::DMA::WIDTH32, /* dst_inc */ false,
                      BUFFER_SIZE, /* irq */ true);

      gpdma.start(dma_channel);
   }

   void stop()
   {
      // TODO
   }
 
   void irqHandler()
   {
      gpdma.clearIrq(dma_channel);

      uint32_t* buffer = ping_pong ? ping : pong;
      ping_pong = not ping_pong;

      DACPump_getSamples(buffer, BUFFER_SIZE);
   }

private:
   MTL::DAC   dac;
   unsigned   dma_channel;
   MTL::GPDMA gpdma {};
   MTL::DMA   dma_ping {};
   MTL::DMA   dma_pong {};
   bool       ping_pong {false};
   uint32_t   ping[BUFFER_SIZE];
   uint32_t   pong[BUFFER_SIZE];
};

} // namespace MTL
