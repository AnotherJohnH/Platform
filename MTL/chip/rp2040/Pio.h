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

struct Label
{
    bool     resolved{false};
    uint32_t addr{0};
    uint32_t unresolved{0};
};

enum ShiftDir { SHIFT_RIGHT = 0, SHIFT_LEFT = 1 };

enum Auto     { MANUAL = 0, AUTO_PULL = 1, AUTO_PUSH = 1 };

template <unsigned INDEX, uint32_t BASE_ADDRESS>
class PioBase : public Periph<PioReg, BASE_ADDRESS>
{
public:
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
   }

   //! Set state machine SET pins
   void SM_pinSET(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      this->setField(this->reg->sm[sd].pinctrl,  9,  5, pin);
      this->setField(this->reg->sm[sd].pinctrl, 28, 26, n);
   }

   //! Set state machine side set pins
   void SM_pinSDE(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      this->setField(this->reg->sm[sd].pinctrl, 14, 10, pin);
      this->setField(this->reg->sm[sd].pinctrl, 31, 29, n);

      this->setBit(this->reg->sm[sd].execctrl, 29);
      SM_exec(sd, NOP().side((1 << n) - 1, n).op());
      this->clrBit(this->reg->sm[sd].execctrl, 29);
   }

   //! Set state machine INP pin
   void SM_pinINP(unsigned sd, unsigned pin)
   {
      configIn(pin, 1);

      this->setField(this->reg->sm[sd].pinctrl, 19, 15, pin);
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
   void SM_configOSR(unsigned sd, unsigned bits, ShiftDir dir, Auto autopull)
   {
      bits &= 0x1F;

      this->reg->sm[sd].shiftctrl = (   1     << 30) |    // Combine TX + RX FIFO
                                    (bits     << 25) |
                                    (dir      << 19) |
                                    (autopull << 17);
   }

   //! Configure input shift register
   void SM_configISR(unsigned sd, unsigned bits, ShiftDir dir, Auto autopush)
   {
      bits &= 0x1F;

      this->reg->sm[sd].shiftctrl = (   1     << 31) |   // Combine RX & TX FIFO
                                    (bits     << 20) |
                                    (dir      << 18) |
                                    (autopush << 16);
   }

   //! Execute an instruction
   void SM_exec(unsigned sd, uint32_t op)
   {
      while(this->getBit(this->reg->sm[sd].execctrl, 31) != 0);

      this->reg->sm[sd].instr = op;
   }

   //! Push data into TX FIFO
   void SM_push(unsigned sd, uint32_t data)
   {
      this->reg->txf[sd] = data;
   }

   //! Pop data from RX FIFO
   uint32_t SM_pop(unsigned sd)
   {
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

      SM_wrap(sd, code.getWrapTarget() + start, code.getWrap() + start);
      SM_exec(sd, JMP(PIO::ALWAYS, code.getEntry() + start).op());
   }

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

   PioBase& JMP(PIO::JmpCond cond, unsigned addr5)
   {
      return code(PIO::Asm::OP_JMP, (cond << 5) | addr5);
   }

   PioBase& JMP(unsigned addr5)
   {
      return JMP(PIO::ALWAYS, addr5);
   }

   PioBase& WFC(PIO::WaitCond cond, unsigned index5)
   {
      return code(PIO::Asm::OP_WFC, (cond << 5) | index5);
   }

   PioBase& INP(PIO::Operand source, unsigned bit_count = 1)
   {
      return code(0b010, (source << 5) | bit_count);
   }

   PioBase& OUT(PIO::Operand dest, unsigned bit_count = 1)
   {
      return code(0b011, (dest << 5) | bit_count);
   }

   PioBase& PSH(PIO::FifoMode mode = PIO::BLOCK)
   {
      return code(0b100, mode);
   }

   PioBase& POP(PIO::FifoMode mode = PIO::BLOCK)
   {
      return code(0b100, (1 << 7) | mode);
   }

   PioBase& MOV(PIO::Operand dest, PIO::Operand source)
   {
      return code(0b101, (dest << 5) | (0b00 << 3) | source);
   }

   PioBase& INV(PIO::Operand dest, PIO::Operand source)
   {
      return code(0b101, (dest << 5) | (0b01 << 3) | source);
   }

   PioBase& REV(PIO::Operand dest, PIO::Operand source)
   {
      return code(0b101, (dest << 5) | (0b10 << 3) | source);
   }

   PioBase& IRQ(PIO::Irq mode, unsigned irq_num)
   {
      return code(0b110, (mode << 5) | irq_num);
   }

   PioBase& SET(PIO::Operand dest, unsigned imm5)
   {
      return code(0b111, (dest << 5) | imm5);
   }

   PioBase& NOP()
   {
      return MOV(PIO::Y, PIO::Y);
   }

   //! Set side set bits in the instruction just written
   PioBase& side(unsigned bits, unsigned side_set_bits)
   {
      assert(bits < (1 << side_set_bits));

      inst = inst | (bits << (13 - side_set_bits));

      return *this;
   }

   uint16_t op() const { return inst; }

private:
   PioBase& code(unsigned op_, uint16_t inst_)
   {
      inst = (op_ << 13) | inst_;
      return *this;
   }

   void configOut(unsigned pin, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
      {
         pads_bank.setOut(pin + i, PadsBank::DRIVE_2MA, /* slew_fast */ true);
         io_bank.setFunc( pin + i, INDEX == 0 ? IoBank::PIO0 : IoBank::PIO1);
      }
   }

   void configIn(unsigned pin, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
      {
         pads_bank.setIn(pin + i);
         io_bank.setFunc(pin + i, INDEX == 0 ? IoBank::PIO0 : IoBank::PIO1);
      }
   }

   PadsBank pads_bank{};
   IoBank   io_bank{};
   uint16_t inst {0};

   static const uint8_t NUM_STATE_MACHINE = 4;

   static uint8_t free_state_machine;
   static uint8_t free_pc;
};

template <unsigned INDEX, uint32_t BASE_ADDRESS>
uint8_t PioBase<INDEX,BASE_ADDRESS>::free_state_machine = 0;

template <unsigned INDEX, uint32_t BASE_ADDRESS>
uint8_t PioBase<INDEX,BASE_ADDRESS>::free_pc = 0;

class Pio0 : public PioBase<0,0x50200000> {};

class Pio1 : public PioBase<1,0x50300000> {};

} // namespace MTL
