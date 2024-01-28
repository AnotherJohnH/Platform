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
// \brief RP2040 I2C peripheral

#pragma once

#include "MTL/Periph.h"

#include "PadsBank.h"
#include "Pll.h"
#include "IoBank.h"
#include "Pins.h"
#include "Resets.h"

namespace MTL {

struct I2CReg
{
   uint32_t con;
   uint32_t tar;
   uint32_t sar;
   uint32_t _pad1;
   uint32_t data_cmd;
   uint32_t ss_scl_hcnt;
   uint32_t ss_scl_lcnt;
   uint32_t fs_scl_hcnt;
   uint32_t fs_scl_lcnt;
   uint32_t _pad2;
   uint32_t _pad3;
   uint32_t intr_stat;
   uint32_t intr_mask;
   uint32_t raw_intr_stat;
   uint32_t rx_tl;
   uint32_t tx_tl;
   uint32_t clr_intr;
   uint32_t clr_rx_under;
   uint32_t clr_rx_over;
   uint32_t clr_tx_over;
   uint32_t clr_rd_req;
   uint32_t clr_tx_abrt;
   uint32_t clr_rx_done;
   uint32_t clr_activity;
   uint32_t clr_stop_det;
   uint32_t clr_start_det;
   uint32_t clr_gen_call;
   uint32_t enable;
   uint32_t status;
   uint32_t txflr;
   uint32_t rxflr;
   uint32_t sda_hold;
   uint32_t tx_abrt_source;
   uint32_t slv_data_nack_only;
   uint32_t dma_cr;
   uint32_t dma_tdlr;
   uint32_t dma_rdlr;
   uint32_t sda_setup;
   uint32_t ack_general_call;
   uint32_t enable_status;
   uint32_t fs_spklen;
   uint32_t _pad4[18];
   uint32_t clr_restart_det;
   uint32_t comp_param1;
   uint32_t comp_version;
   uint32_t comp_type;
};

//! I2C Interface
template <uint32_t BASE_ADDRESS, uint32_t RESET, unsigned SDA_PIN, unsigned SCL_PIN>
class I2C : public Periph<I2CReg, BASE_ADDRESS>
{
public:
   I2C()
   {
      Resets resets;
      resets.setReset(RESET);
      resets.clrReset(RESET);
      
      PadsBank pads_bank;
      IoBank   io_bank;

      // Configure SDA pin
      //pads_bank.setOut(SDA_PIN, PadsBank::DRIVE_2MA | PadsBank::PULL_UP);
      io_bank.setFunc( SDA_PIN, IoBank::I2C);

      // Configure SCL pin
      //pads_bank.setOut(SCL_PIN, PadsBank::DRIVE_2MA | PadsBank::PULL_UP);
      io_bank.setFunc( SCL_PIN, IoBank::I2C);

      this->reg->enable = 0;

      // Assuming the default CON register is
      // + Master 400 KHz 7bit address
      this->reg->con = (1    << 8) |  // TX_EMPTY_CTRL
                       (1    << 6) |  // SLAVE_DIABLE
                       (1    << 5) |  // RESTART_EN
                       (0b10 << 1) |  // SPEED_VALUE_FAST
                       (1    << 0);   // MASTER_MODE

      this->reg->tx_tl = 0;
      this->reg->rx_tl = 0;

#if 0
      unsigned clock_cnt     = PllSys().getFreq() / I2C_FREQ;
      this->reg->fs_scl_hcnt = clock_cnt / 2;
      this->reg->fs_scl_lcnt = clock_cnt / 2;
#else
      // XXX currently a copy of pico SDK code (TBC)
      unsigned freq_in           = PllSys().getFreq();
      unsigned period            = (freq_in + I2C_FREQ/ 2) / I2C_FREQ;
      unsigned lcnt              = period * 3 / 5; // oof this one hurts
      unsigned hcnt              = period - lcnt;
      unsigned sda_tx_hold_count = ((freq_in * 3) / 10000000) + 1;

      this->reg->fs_scl_hcnt = hcnt;
      this->reg->fs_scl_lcnt = lcnt;
      this->reg->fs_spklen   = lcnt < 16 ? 1 : lcnt / 16;
      this->reg->sda_hold    = (this->reg->sda_hold & 0xFFFF0000) | sda_tx_hold_count;
#endif

      this->reg->enable = 1;
    }

    void setTarget(uint8_t address_)
    {
       this->reg->enable = 0;
       this->reg->tar    = address_ & 0x7F;
       this->reg->enable = 1;
    }

    void write(uint8_t data, bool stop)
    {
       while(isTxFIFOfull());

       uint32_t data_cmd = data;
       if (stop) data_cmd |= 1 << 9;
       this->reg->data_cmd = data_cmd;

       // Wait for TX_EMPTY
       while(this->reg->raw_intr_stat & (1 << 4) == 0);
    }

    uint8_t read(bool stop)
    {
       while(isTxFIFOfull());

       uint32_t data_cmd = 1 << 8;;
       if (stop) data_cmd |= 1 << 9;
       this->reg->data_cmd = data_cmd;

       while(isRxFIFOempty());

       return this->reg->data_cmd & 0xFF;
    }

private:
    static const unsigned I2C_FREQ = 400000;

    bool isRxFIFOfull()  const { return this->reg->status & (1 << 4) != 0; }
    bool isRxFIFOempty() const { return this->reg->status & (1 << 3) == 0; }
    bool isTxFIFOempty() const { return this->reg->status & (1 << 2) != 0; }
    bool isTxFIFOfull()  const { return this->reg->status & (1 << 1) == 0; }
};

using I2C0_P1_P2   = I2C<0x40044000, Resets::I2C0, rp2040::IO_PIN_0,  rp2040::IO_PIN_1>;
using I2C0_P11_P12 = I2C<0x40044000, Resets::I2C0, rp2040::IO_PIN_8,  rp2040::IO_PIN_9>;
using I2C0_P16_P17 = I2C<0x40044000, Resets::I2C0, rp2040::IO_PIN_12, rp2040::IO_PIN_13>;
using I2C0_P21_P22 = I2C<0x40044000, Resets::I2C0, rp2040::IO_PIN_16, rp2040::IO_PIN_17>;
using I2C0_P26_P27 = I2C<0x40044000, Resets::I2C0, rp2040::IO_PIN_20, rp2040::IO_PIN_21>;

using I2C1_P4_P5   = I2C<0x40048000, Resets::I2C1, rp2040::IO_PIN_2,  rp2040::IO_PIN_3>;
using I2C1_P9_P10  = I2C<0x40048000, Resets::I2C1, rp2040::IO_PIN_6,  rp2040::IO_PIN_7>;
using I2C1_P19_P20 = I2C<0x40048000, Resets::I2C1, rp2040::IO_PIN_14, rp2040::IO_PIN_15>;
using I2C1_P24_P25 = I2C<0x40048000, Resets::I2C1, rp2040::IO_PIN_18, rp2040::IO_PIN_19>;
using I2C1_P31_P32 = I2C<0x40048000, Resets::I2C1, rp2040::IO_PIN_26, rp2040::IO_PIN_27>;

} // namespace MTL
