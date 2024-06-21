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
// \brief RP2040 USB peripheral

#pragma once

#include <algorithm>
#include <cstring>

#include "MTL/Periph.h"
#include "MTL/UsbTypes.h"
#include "MTL/CortexM0/NVIC.h"
#include "MTL/rp2040/Resets.h"

#define LOG if (0) printf

namespace MTL {

struct DPSRamReg
{
   uint8_t  setup_packet[8];
   uint32_t ep_control[30];     //!< End-point control for EP1:15 IN,OUT
   uint32_t buffer_control[32]; //!< End-point buffer control for EP0:15 IN,OUT
   uint8_t  ep0_buffer0[0x40];  //!< EP0 buffer0 (shared for in and out)
   uint8_t  ep0_buffer1[0x40];  //!< EP0 buffer1 (optional)
   uint8_t  data[0xE80];        //!< Data buffers (as allocated)
};

struct USBReg
{
   uint32_t addr_endp[16];
   uint32_t main_ctrl;
   uint32_t sof_wr;
   uint32_t sof_rd;
   uint32_t sie_ctrl;
   uint32_t sie_status;
   uint32_t int_ep_ctrl;
   uint32_t buff_status;
   uint32_t buff_cpu_should_handle;
   uint32_t ep_abort;
   uint32_t ep_abort_done;
   uint32_t ep_stall_arm;
   uint32_t nak_poll;
   uint32_t ep_status_stall_nak;
   uint32_t usb_muxing;
   uint32_t usb_pwr;
   uint32_t usbphy_direct;
   uint32_t usbphy_direct_override;
   uint32_t usbphy_trim;
   uint32_t pad;
   uint32_t intr;
   uint32_t inte;
   uint32_t intf;
   uint32_t ints;
};

//! USB Interface
class Usb : public Periph<USBReg,0x50110000>
{
public:
   class EndPoint : public USB::EndPoint
   {
   public:
      EndPoint() = default;

      EndPoint(USB::Descr& descr_)
         : USB::EndPoint(descr_)
      {
      }

      //! Set next PID as DATA1
      void setPID()
      {
         pid_bit = BC0_PID_DATA1;
      }

      //! Start an outgoing transimission from the Device to Host
      void startTx(unsigned len_)
      {
         *control = BC0_FULL | pid_bit | BC0_AVAIL | len_;
         pid_bit ^= BC0_PID_DATA1;
      }

      //! Start an incoming transmission from the Host to Device
      void startRx(unsigned len_)
      {
         *control = pid_bit | BC0_AVAIL | len_;
         pid_bit ^= BC0_PID_DATA1;
      }

      //! Start an acknowledge back to the host
      void startAck()
      {
         startTx(0);
      }

      //! Write data into buffer
      unsigned write(const void* data, unsigned length, unsigned offset = 0)
      {
         memcpy((void*)&buffer[offset], data, length);
         offset += length;
         return offset;
      }

      static const uint32_t BC0_FULL      = 1 << 15;
      static const uint32_t BC0_LAST      = 1 << 14;
      static const uint32_t BC0_PID_DATA1 = 1 << 13;
      static const uint32_t BC0_RESET     = 1 << 12;
      static const uint32_t BC0_STALL     = 1 << 11;
      static const uint32_t BC0_AVAIL     = 1 << 10;

      volatile uint32_t* control{};   //!< Buffer control register
      volatile uint8_t*  buffer{};    //!< Buffer
      uint32_t           pid_bit{};   //!< Next PID state for buffer control
   };

   Usb(USB::Device& device_)
      : device(&device_)
   {
       MTL::Resets resets;

       // Reset
       resets.setReset(MTL::Resets::USBCTRL);
       resets.clrReset(MTL::Resets::USBCTRL);

       // Clear DPRam
       memset((void*)ram.reg, 0, sizeof(DPSRamReg));

       // Enable USBCTRL interrupt
       MTL::NVIC<5>().enable();

       // Mux the controller to the onboard USB phy
       reg->usb_muxing = (1 << 3) |    // SOFTCON
                         (1 << 0);     // TO_PHY

       // Force VBUS detect for plugged into a host
       reg->usb_pwr    = (1 << 3) |    // VBUS_DETECT_OVERRIDE_EN
                         (1 << 2);     // VBUS_DETECT_OVERRIDE

       // Enable controller in host or dev mode
       reg->main_ctrl  = 0b01;  // DEVICE + ENABLE

       // Enable an interrupt per EP0 transaction
       reg->sie_ctrl   = 1 << 29;      // EP0_INT_1BUF

       // Interrupt on buffer done, bus reset and setup
       reg->inte = INT_BUFF_STATUS |
                   INT_BUS_RESET   |
                   INT_SETUP_REQ;

       // Setup device control end-points
       initEndPoint(&ep0_in,  USB::EndPointDescr::IN,  USB::EndPointDescr::CONTROL);
       initEndPoint(&ep0_out, USB::EndPointDescr::OUT, USB::EndPointDescr::CONTROL);

       // Present a full-speed device by enabling the pull-up
       reg->sie_ctrl  |= 1 << 16;      // PULLUP_EN
   }

   //! Handle interrupt
   void irq()
   {
      uint32_t active = reg->ints;

      if (active & INT_SETUP_REQ)
      {
         handleSetupReq();
      }

      if (active & INT_BUS_RESET)
      {
         handleBusReset();
      }

      if (active & INT_BUFF_STATUS)
      {
         handleBuffStatus();
      }
   }

private:
   Periph<DPSRamReg,0x50100000> ram;

   //! Initialise an end-point
   void initEndPoint(EndPoint* endpoint_, uint8_t addr_, uint8_t type_)
   {
      // Compute register index
      unsigned ep_index = addr_ & 0b00001111;
      unsigned rg_index = (ep_index << 1) | ((addr_ >> 7) ^ 0b1);

      endpoint_->control = &ram.reg->buffer_control[rg_index];

      if (ep_index == 0)
      {
         endpoint_->buffer = ram.reg->ep0_buffer0;
      }
      else
      {
         ram.reg->ep_control[rg_index - 2] = (1 << 31) | // ENABLE
                                             (1 << 29) | // INTERRUPT_PER_BUFFER_TRF
                                             (type_ << 26) |
                                             dpram_offset;

         endpoint_->buffer = (volatile uint8_t*)(ram.reg) + dpram_offset;

         // XXX 64 may not always be the right value here
         dpram_offset += 64;
      }
   }

   void handleBusReset()
   {
      reg->sie_status |= (1 << 19);  // Clear request
      reg->addr_endp[0] = 0;         // Clear address and endpoint control

      LOG("\n>>>> BUS RESET\n");
   }

   void handleGetDeviceDescr(USB::SetupReq* packet)
   {
      ep0_in.setPID();

      unsigned bytes = ep0_in.write(&device->descr, device->descr.length);
      ep0_in.startTx(std::min(bytes, unsigned(packet->length)));

      LOG("GET_DESCR DEV %u\n", packet->length);
   }

   void handleGetConfigDescr(USB::SetupReq* packet)
   {
      USB::Config* config = device->getConfig(1);

      config->linkDescriptors();

      buffer.clear();
      buffer.write(&config->descr.length, config->descr.length);

      if (packet->length == config->descr.total_length)
      {
         for(auto& interface : config->interface_list)
         {
            for(auto& d : interface.descr_list)
            {
               buffer.write(&d.getLength(), d.getLength());
            }
         }
      }

      size_t bytes = std::min(size_t(64), buffer.size());
      ep0_in.write(buffer.read(bytes), bytes);
      ep0_in.startTx(bytes);

      LOG("GET_DESCR CFG %u/%u\n", bytes, packet->length);
   }

   void handleGetStringDescr(USB::SetupReq* packet)
   {
      uint8_t        id     = packet->value & 0xFF;
      const uint8_t* string = device->getString(id);
      uint8_t        len    = *string++;

      buffer.clear();

      if (id == 0)
      {
         // Language descriptor
         buffer.write(4);
         buffer.write(USB::STRING);
         buffer.write(string[0]);
         buffer.write(string[1]);
      }
      else
      {
         buffer.write(2 + len * 2);
         buffer.write(USB::STRING);

         for(unsigned i = 0; i < len; ++i)
         {
            if (buffer.size() == packet->length)
               break;

            buffer.write(string[i]);
            buffer.write(0x00);
         }
      }

      size_t bytes = std::min(size_t(64), buffer.size());
      ep0_in.write(buffer.read(bytes), bytes);
      ep0_in.startTx(bytes);

      LOG("GET_DESCR STR id=%u bytes=%u\n", id, bytes);
   }

   void handleSetAddress(USB::SetupReq* packet)
   {
      // Don't change the address until the buffer is sent
      set_address = true;
      address     = packet->value & 0xFF;

      LOG("SET_ADDRESS %02x\n", address);
   }

   void handleSetConfig(USB::SetupReq* packet)
   {
      USB::Config* config = device->getConfig(packet->value & 0xFF);

      dpram_offset = 0x180;

      for(auto& interface : config->interface_list)
      {
         for(const auto& descr : interface.descr_list)
         {
            if (descr.getType() == USB::ENDPOINT)
            {
               const USB::EndPointDescr* ep_descr = (USB::EndPointDescr*)&descr;
               EndPoint*                 ep = (EndPoint*) ep_descr->getImpl();
               initEndPoint(ep, ep_descr->addr, ep_descr->attr);
            }
         }

         interface.configured();
      }

      LOG("SET_CONFIG %u\n", packet->value);
   }

   void handleSetupReq()
   {
      reg->sie_status |= (1 << 17);  // Clear request

      ep0_in.setPID();

      USB::SetupReq* packet = (USB::SetupReq*)ram.reg->setup_packet;

      if (packet->isHostToDevice())
      {
         switch(packet->request)
         {
         case USB::Request::SET_ADDRESS: handleSetAddress(packet); break;
         case USB::Request::SET_CONFIG:  handleSetConfig(packet);  break;

         default:
            LOG("SETUP OTHER IN %u\n", packet->request);
            break;
         }

         ep0_in.startAck();
      }
      else
      {
         switch(packet->request)
         {
         case USB::Request::GET_DESCRIPTOR:
            switch(packet->value >> 8)
            {
            case USB::DEVICE: handleGetDeviceDescr(packet); break;
            case USB::CONFIG: handleGetConfigDescr(packet); break;
            case USB::STRING: handleGetStringDescr(packet); break;
            }
            break;

         default:
            LOG("SETUP OTHER OUT %u\n", packet->request);
            break;
         }
      }
   }

   void handleBuffStatus()
   {
      uint32_t buffer_mask = reg->buff_status;

      for(unsigned bit = 0; bit < 32; bit++)
      {
          uint32_t mask = 1 << bit;

          if (buffer_mask & mask)
          {
              bool     is_tx = (bit & 1) == 0;
              unsigned ep    = bit / 2;

              if (ep == 0)
              {
                 if (is_tx)
                 {
                    if (set_address)
                    {
                       reg->addr_endp[0] = address;
                       set_address = false;
                    }
                    else if (not buffer.empty())
                    {
                       size_t bytes = std::min(size_t(64), buffer.size());
                       ep0_in.write(buffer.read(bytes), bytes);
                       ep0_in.startTx(bytes);
                       LOG("TX remaining %u\n", bytes);
                    }
                    else
                    {
                       ep0_out.startRx(0);
                    }
                 }
              }
              else
              {
                 if (is_tx)
                 {
                    device->handleBuffTx(ep);
                 }
                 else
                 {
                    unsigned       len    = ram.reg->buffer_control[bit] & 0x3FF;
                    unsigned       offset = ram.reg->ep_control[bit - 2] & 0xFC0;
                    const uint8_t* buffer = (const uint8_t*)(ram.reg) + offset;

                    device->handleBuffRx(ep, buffer, len);
                 }
              }

              buffer_mask &= ~mask;
              reg->buff_status = mask;
              if (buffer_mask == 0)
                  break;
          }
      }
   }

   template <size_t N>
   class Buffer
   {
   public:
      size_t size() const { return write_ptr - read_ptr; }

      bool empty() const { return size() == 0; }

      void clear() { write_ptr = read_ptr = 0; }

      void write(uint8_t byte_)
      {
         raw[write_ptr++] = byte_;
      }

      void write(const uint8_t* raw_, size_t size_)
      {
         memcpy(raw + write_ptr, raw_, size_);
         write_ptr += size_;
      }

      const uint8_t* read(size_t size_)
      {
         const uint8_t* ptr = raw + read_ptr;
         read_ptr += std::min(size_, size());
         return ptr;
      }

   private:
      uint8_t write_ptr{0};
      uint8_t read_ptr{0};
      uint8_t raw[N];
   };

   static const uint32_t INT_SETUP_REQ   = 1 << 16;
   static const uint32_t INT_BUS_RESET   = 1 << 12;
   static const uint32_t INT_BUFF_STATUS = 1 <<  4;

   USB::Device* device;
   EndPoint     ep0_in{};
   EndPoint     ep0_out{};
   uint32_t     dpram_offset{0x180};  // XXX what about 0x140-0x17F

   bool         set_address{false};
   uint8_t      address{};
   Buffer<128>  buffer{};
};

} // namespace MTL
