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

// \brief RP2040 PIO peripheral

#pragma once

#include <cassert>

#include "MTL/Periph.h"

#include "PioAsm.h"

#include "PadsBank.h"
#include "Pll.h"
#include "IoBank.h"

namespace MTL {

struct PioReg
{
   uint32_t ctrl;
   uint32_t fstat;
   uint32_t fdebug;
   uint32_t flevel;
   uint32_t txf[4];
   uint32_t rxf[4];
   uint32_t irq;
   uint32_t irq_force;
   uint32_t input_sync_bypass;
   uint32_t dbg_padout;
   uint32_t dbg_padoe;
   uint32_t dbg_cfginfo;
   uint32_t instr_mem[32];

   struct StateMachine
   {
      uint32_t clkdiv;
      uint32_t execctrl;
      uint32_t shiftctrl;
      uint32_t addr;
      uint32_t instr;
      uint32_t pinctrl;
   };

   StateMachine sm[4];
   uint32_t     intr;
   uint32_t     irq0_inte;
   uint32_t     irq0_intf;
   uint32_t     irq0_ints;
   uint32_t     irq1_inte;
   uint32_t     irq1_intf;
   uint32_t     irq1_ints;
};

enum ShiftDir { SHIFT_RIGHT = 1, SHIFT_LEFT = 0 };

enum Auto { MANUAL = 0, AUTO_PULL = 1, AUTO_PUSH = 1 };

template <IoBank::Func IO_FUNC, uint32_t BASE_ADDRESS, unsigned DREQ_BASE>
class Pio : public Periph<PioReg, BASE_ADDRESS>
          , public PIO::AsmBase<1>
{
public:
   Pio() = default;

   //! Allocate the next free state machine
   signed allocSM()
   {
      if (free_state_machine == NUM_STATE_MACHINE)
         return -1;

      return free_state_machine++;
   }

   void SM_pinOUT(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      this->setField(this->reg->sm[sd].pinctrl,  4,  0, pin);
      this->setField(this->reg->sm[sd].pinctrl, 25, 20, n);

      // Set pin direction using OUT pins
      unsigned mask = (1 << n) - 1;
      SM_push(sd, mask);
      SM_exec(sd, POP().op());
      SM_exec(sd, OUT(PIO::PINDIRS, n).op());
   }

   //! Set state machine SET pins
   void SM_pinSET(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      // Get PINCTRL
      uint32_t pinctrl = this->reg->sm[sd].pinctrl;

      // Configure SET pins
      this->setField(pinctrl,  9,  5, pin);
      this->setField(pinctrl, 28, 26, n);

      // Configure OUT pins as SET pins
      uint32_t tmp_pinctrl = pinctrl;
      this->setField(tmp_pinctrl,  4,  0, pin);
      this->setField(tmp_pinctrl, 25, 20, n);
      this->reg->sm[sd].pinctrl = tmp_pinctrl;

      // Set SET pin direction using OUT pins
      unsigned mask = (1 << n) - 1;
      SM_push(sd, mask);
      SM_exec(sd, POP().op());
      SM_exec(sd, OUT(PIO::PINDIRS, n).op());

      // Set PINCTRL (restores previous OUT config)
      this->reg->sm[sd].pinctrl = pinctrl;
   }

   //! Set state machine SIDE set pins
   void SM_pinSIDE(unsigned sd, unsigned pin)
   {
      unsigned n = getSideSetBits();

      configOut(pin, n);

      // Get PINCTRL
      uint32_t pinctrl = this->reg->sm[sd].pinctrl;

      // Configure SIDE pins
      this->setField(pinctrl, 14, 10, pin);

      if (getSideSetEnable())
      {
          this->setBit(this->reg->sm[sd].execctrl, 30);
          this->setField(pinctrl, 31, 29, n + 1);
      }
      else
      {
          this->setField(pinctrl, 31, 29, n);
      }
      this->setField(pinctrl, 28, 26, n);

      // Configure OUT pins as SIDE pins
      uint32_t tmp_pinctrl = pinctrl;
      this->setField(tmp_pinctrl,  4,  0, pin);
      this->setField(tmp_pinctrl, 25, 20, n);
      this->reg->sm[sd].pinctrl = tmp_pinctrl;

      // Set SIDE pin direction using OUT pins
      unsigned mask = (1 << n) - 1;
      SM_push(sd, mask);
      SM_exec(sd, POP().op());
      SM_exec(sd, OUT(PIO::PINDIRS, n).op());

      // Set PINCTRL (restores previous OUT config)
      this->reg->sm[sd].pinctrl = pinctrl;
   }

   //! Set state machine INP pin
   void SM_pinINP(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configIn(pin, n);

      // Get PINCTRL
      uint32_t pinctrl = this->reg->sm[sd].pinctrl;

      // Configure INP pins
      this->setField(pinctrl, 19, 15, pin);

      // Configure OUT pins as INP pins
      uint32_t tmp_pinctrl = pinctrl;
      this->setField(tmp_pinctrl,  4,  0, pin);
      this->setField(tmp_pinctrl, 25, 20, n);
      this->reg->sm[sd].pinctrl = tmp_pinctrl;

      // Set INP pin direction using OUT pins
      SM_exec(sd, MOV(PIO::OSR, PIO::ZERO).op());
      SM_exec(sd, OUT(PIO::PINDIRS, n).op());

      // Set PINCTRL (restores previous OUT config)
      this->reg->sm[sd].pinctrl = pinctrl;
   }

   //! Set state machine clock
   void SM_clock(unsigned sd, unsigned freq)
   {
      uint32_t clkdiv8 = uint64_t(PllSys().getFreq()) * 256 / freq;

      assert((clkdiv8 >= 0x100) && (clkdiv8 <= 0xFFFFFF));

      this->reg->sm[sd].clkdiv = clkdiv8 << 8;
   }

   //! Set state machine wrap points
   void SM_wrap(unsigned sd, unsigned btm, unsigned top)
   {
      this->setField(this->reg->sm[sd].execctrl, 16, 7, ((top - 1) << 5) | btm);
   }

   //! Configure output shift register
   void SM_configOSR(unsigned sd,
                     unsigned bits,
                     ShiftDir dir,
                     Auto     autopull,
                     bool     join_tx)
   {
      bits &= 0x1F;

      uint32_t mask = ( 1 << 30) |
                      (31 << 25) |
                      ( 1 << 19) |
                      ( 1 << 17) |
                      ( 1 << 15);

      uint32_t shiftctrl = this->reg->sm[sd].shiftctrl & ~mask;

      this->reg->sm[sd].shiftctrl = shiftctrl                 |
                                    ((join_tx ? 1 : 0) << 30) |
                                    (bits              << 25) |
                                    (dir               << 19) |
                                    (autopull          << 17) |
                                    (0                 << 15);
   }

   //! Configure input shift register
   void SM_configISR(unsigned sd,
                     unsigned bits,
                     ShiftDir dir,
                     Auto     autopush,
                     bool     join_rx)
   {
      bits &= 0x1F;

      uint32_t mask = ( 1 << 31) |
                      (31 << 20) |
                      ( 1 << 18) |
                      ( 1 << 16) |
                      ( 1 << 14);

      uint32_t shiftctrl = this->reg->sm[sd].shiftctrl & ~mask;

      this->reg->sm[sd].shiftctrl = shiftctrl                 |
                                    ((join_rx ? 1 : 0) << 31) |
                                    (bits              << 20) |
                                    (dir               << 18) |
                                    (autopush          << 16) |
                                    (0                 << 14);
   }

   //! Execute an instruction
   void SM_exec(unsigned sd, uint32_t op)
   {
      while(this->getBit(this->reg->sm[sd].execctrl, 31) != 0);

      this->reg->sm[sd].instr = op;
   }

   //! Push data into TX FIFO
   void SM_push(unsigned sd, uint32_t data) const
   {
      // Block if FIFO full
      while(this->getBit(this->reg->fstat, 16 + sd) != 0);

      this->reg->txf[sd] = data;
   }

   //! Pop data from RX FIFO
   uint32_t SM_pop(unsigned sd)
   {
      // Block if FIFO empty
      while(this->getBit(this->reg->fstat, 8 + sd) != 0);

      return this->reg->rxf[sd];
   }

   //! Program a state machine
   void SM_program(unsigned sd, PIO::Asm& code)
   {
      uint8_t start = free_pc;

      for(unsigned i = 0; i < code.size(); i++)
      {
         uint32_t inst = code[i];

         if ((inst >> 13) == PIO::Asm::OP_JMP)
         {
             // Relocate JMPs
             uint8_t target = (inst & 0x1F) + start;;
             inst = (inst & ~0x1f) | target;
         }

         this->reg->instr_mem[free_pc++] = inst;
      }

      side_set(code.getSideSetBits(), code.getSideSetEnable());

      SM_wrap(sd, code.getWrapTarget() + start, code.getWrap() + start);
      SM_exec(sd, JMP(code.getEntry() + start).op());
   }

   //! Return TX FIFO
   volatile uint32_t* SM_getTxFIFO(unsigned sd) const { return &this->reg->txf[sd]; }

   //! Return RX FIFO
   volatile uint32_t* SM_getRxFIFO(unsigned sd) const { return &this->reg->rxf[sd]; }

   //! Return DMA DREQ for TX FIFO
   unsigned SM_getTxDREQ(unsigned sd) const { return DREQ_BASE + 0 + sd; }

   //! Return DMA DREQ for RX FIFO
   unsigned SM_getRxDREQ(unsigned sd) const { return DREQ_BASE + 4 + sd; }

   //! Start state machines
   void start(unsigned sd_mask)
   {
      // Reset clocks
      this->setField(this->reg->ctrl, 11, 8, this->reg->ctrl | sd_mask);

      // Start running
      this->setField(this->reg->ctrl,  3, 0, this->reg->ctrl | sd_mask);
   }

   //! Stop state machines
   void stop(unsigned sd_mask)
   {
      this->setField(this->reg->ctrl, 3, 0, this->reg->ctrl & ~sd_mask);
   }

private:
   void configOut(unsigned pin, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
      {
         io_bank.setFunc( pin + i, IO_FUNC);
         pads_bank.setOut(pin + i, PadsBank::DRIVE_4MA, /* slew_fast */ true);
      }
   }

   void configIn(unsigned pin, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
      {
         io_bank.setFunc(pin + i, IO_FUNC);
         pads_bank.setIn(pin + i);
      }
   }

   PadsBank pads_bank{};
   IoBank   io_bank{};

   static const uint8_t NUM_STATE_MACHINE = 4;

   static uint8_t free_state_machine;
   static uint8_t free_pc;
};

template <IoBank::Func IO_FUNC, uint32_t BASE_ADDRESS, unsigned DREQ_BASE>
uint8_t Pio<IO_FUNC,BASE_ADDRESS,DREQ_BASE>::free_state_machine = 0;

template <IoBank::Func IO_FUNC, uint32_t BASE_ADDRESS, unsigned DREQ_BASE>
uint8_t Pio<IO_FUNC,BASE_ADDRESS,DREQ_BASE>::free_pc = 0;

using Pio0 = Pio<IoBank::PIO0,0x50200000,/* DREQ_BASE */  0>;
using Pio1 = Pio<IoBank::PIO1,0x50300000,/* DREQ_BASE */  8>;

#if defined(MTL_RP2350)
using Pio2 = Pio<IoBank::PIO1,0x50400000,/* DREQ_BASE */ 16>;
#endif

} // namespace MTL
