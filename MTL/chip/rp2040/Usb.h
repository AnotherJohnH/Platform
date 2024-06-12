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
#include "MTL/USBTypes.h"
#include "MTL/CortexM0/NVIC.h"
#include "MTL/rp2040/Resets.h"

#define LOG if (1) printf

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
      using USB::EndPoint::EndPoint;

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
      unsigned writeBytes(const void* data, unsigned length, unsigned offset = 0)
      {
         memcpy((void*)&buffer[offset], data, length);
         offset += length;
         return offset;
      }

      //! Write data into buffer
      template<typename TYPE>
      unsigned write(const TYPE& data, unsigned offset = 0)
      {
         return writeBytes(&data, sizeof(TYPE), offset);
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
       setupEndPoint(&ep0_in, 0);
       setupEndPoint(&ep0_out, 0);

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
   void setupEndPoint(EndPoint* endpoint_, unsigned ep_index)
   {
      // TODO set end point index in descriptor
      // Compute register index
      ep_index = endpoint_->descr.addr & 0xF;
      unsigned rg_index = (ep_index << 1) | ((endpoint_->descr.addr >> 7) ^ 0b1);

      endpoint_->control = &ram.reg->buffer_control[rg_index];

      if (ep_index == 0)
      {
         endpoint_->buffer = ram.reg->ep0_buffer0;
      }
      else
      {
         ram.reg->ep_control[rg_index] = (1 << 31) | // ENABLE
                                         (1 << 29) | // INTERRUPT_PER_BUFFER_TRF
                                         (endpoint_->descr.attr << 26) |
                                         dpram_offset;

         endpoint_->buffer = (volatile uint8_t*)(&ram.reg + dpram_offset);

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

      unsigned bytes = ep0_in.write(device->descr);
      ep0_in.startTx(std::min(bytes, unsigned(packet->length)));

      LOG("GET_DESCR DEV %u\n", packet->length);
   }

   void handleGetConfigDescr(USB::SetupReq* packet)
   {
      USB::Config* config = device->getConfig(1);

      unsigned offset = ep0_in.write(config->descr);

      if (packet->length == config->descr.total_length)
      {
         for(USB::Interface* interface = config->getFirstInterface();
             interface != nullptr;
             interface = interface->getNext())
         {
            offset = ep0_in.write(interface->descr, offset);

            for(USB::CSInterface* cs_interface = interface->getFirstCSInterface();
                cs_interface;
                cs_interface = cs_interface->getNext())
            {
#if 0
               offset = ep0_in.writeBytes(cs_interface->getDescr(), cs_interface->getSize(), offset);
#endif
            }

            for(USB::EndPoint* end_point = interface->getFirstEndPoint();
                 end_point!= nullptr;
                 end_point = end_point->getNext())
            {
               offset = ep0_in.write(end_point->descr, offset);

               const uint8_t* cs_descr = end_point->getCSDescr();
               if (cs_descr != nullptr)
               {
#if 0
                  offset = ep0_in.writeBytes(cs_descr, cs_descr[0], offset);
#endif
               }
            }
         }
      }

      ep0_in.startTx(std::min(offset, unsigned(packet->length)));

      LOG("GET_DESCR CFG %u\n", packet->length);
   }

   void handleGetStringDescr(USB::SetupReq* packet)
   {
      uint8_t        id     = packet->value & 0xFF;
      const uint8_t* string = device->getString(id);
      uint8_t        len    = *string++;
      uint8_t        bytes;

      if (id == 0)
      {
         uint8_t lang_desc[4] = {4, USB::STRING, string[0], string[1]};

         bytes = std::min(lang_desc[0], uint8_t(packet->length));

         ep0_in.writeBytes(lang_desc, bytes);
      }
      else
      {
         uint8_t pair[2] = {uint8_t(2 + len * 2), USB::STRING};

         bytes = std::min(pair[0], uint8_t(packet->length));

         for(unsigned offset = ep0_in.writeBytes(pair, 2);
             offset < bytes;
             offset = ep0_in.writeBytes(pair, 2, offset))
         {
            pair[0] = *string++;
            pair[1] = '\0';
         }
      }

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

      unsigned ep_index = 1;

      dpram_offset = 0x180;

      for(USB::Interface* interface = config->getFirstInterface();
          interface != nullptr;
          interface = interface->getNext())
      {
         for(USB::EndPoint* end_point = interface->getFirstEndPoint();
             end_point!= nullptr;
             end_point = end_point->getNext())
         {
            setupEndPoint((EndPoint*)end_point, ep_index++);
         }
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
              bool     is_rx = (bit & 1) == 0;
              unsigned ep    = bit / 2;

              if (ep == 0)
              {
                 if (is_rx)
                 {
                    if (set_address)
                    {
                       reg->addr_endp[0] = address;
                       set_address = false;
                    }
                    else
                    {
                       ep0_out.startRx(0);
                    }
                 }
              }
              else
              {
                 if (is_rx)
                 {
                    unsigned       len    = ram.reg->buffer_control[bit] & 0x3FF;
                    unsigned       offset = ram.reg->ep_control[bit] & 0xFC0;
                    const uint8_t* buffer = (const uint8_t*)(&ram.reg + offset);

                    device->handleBuffRx(ep, buffer, len);
                 }
                 else
                 {
                    device->handleBuffTx(ep);
                 }
              }

              buffer_mask &= ~mask;
              reg->buff_status = mask;
              if (buffer_mask == 0)
                  break;
          }
      }
   }

   static const uint32_t INT_SETUP_REQ   = 1 << 16;
   static const uint32_t INT_BUS_RESET   = 1 << 12;
   static const uint32_t INT_BUFF_STATUS = 1 <<  4;

   USB::Device* device;
   EndPoint     ep0_in{ USB::EndPointDescr::IN};
   EndPoint     ep0_out{USB::EndPointDescr::OUT};
   uint32_t     dpram_offset{0x180};  // XXX what about 0x140-0x17F

   bool         set_address{false};
   uint8_t      address{};
};

} // namespace MTL
