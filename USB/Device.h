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
// \brief USB device types

#pragma once

#include "USB/Descr.h"
#include "USB/Interface.h"

namespace USB {

class Device
{
public:
   Device(const char* vendor_name_,
          uint16_t    product_id_,
          uint16_t    bcd_version_,
          const char* product_name_,
          const char* serial_number_)
   {
      // Set English (US) as the language descriptor
      addString("\x09\x04", 2);

      device_descr.vendor_idx     = addString(vendor_name_);
      device_descr.product_id     = product_id_;
      device_descr.device_bcd     = bcd_version_;
      device_descr.product_idx    = addString(product_name_);
      device_descr.serial_num_idx = addString(serial_number_);
      device_descr.num_configs    = 1;

      config_descr.attributes = 0b10100000; // USB 2.0 Bus Powered, no remote wakeup
      config_descr.max_power  = 50;         // 100 mA
   }

   void setClassAndProtocol(uint8_t class_, uint8_t sub_class_, uint8_t protocol_ = 0)
   {
      device_descr.clas      = class_;
      device_descr.sub_class = sub_class_;
      device_descr.protocol  = protocol_;
   }

   void setConfig(unsigned config_num_)
   {
      config_num = config_num_;
   }

   //! Get a USB descriptor string
   const uint8_t* getString(uint8_t idx_) const { return &string_buffer[idx_]; }

   //! Get the list of interfaces for the selected config
   STB::List<Interface>& getInterfaceList() { return interface_list; }

   //! Incoming data from physical device
   void handleBuffRx(uint8_t ep, const uint8_t* buffer, unsigned length)
   {
      if (buffer_handler[ep] != nullptr)
      {
         buffer_handler[ep]->buffRx(ep, buffer, length);
      }
   }

   //! Data just sent by physical device
   void handleBuffTx(uint8_t ep)
   {
      if (buffer_handler[ep] != nullptr)
      {
         buffer_handler[ep]->buffTx(ep);
      }
   }

   //! declares interfaces in the config descriptor and assigned end points
   void linkDescriptors();

protected:
   DeviceDescr          device_descr{};
   ConfigDescr          config_descr{};  //!< XXX Just one config
   STB::List<Interface> interface_list{};
   unsigned             config_num{0};

private:
   //! Allocate a StringDescr for a string \return index
   uint8_t addString(const char* string_, size_t length = 0)
   {
      uint8_t  idx    = string_idx;
      uint8_t* buffer = &string_buffer[idx];

      if (length == 0)
         length = strlen(string_);

      buffer[0] = length;
      memcpy(buffer + 1, string_, length);
      string_idx += 1 + length;

      return idx;
   }

   uint8_t     string_idx{0};
   uint8_t     string_buffer[256];
   Interface*  buffer_handler[16] = {};
};


void Device::linkDescriptors()
{
   if (config_descr.total_length != 0)
      return;

   uint8_t ep_addr = 1;

   config_descr.num_interfaces = 0;
   config_descr.total_length   = config_descr.length;

   for(auto& interface : interface_list)
   {
      InterfaceDescr* interface_descr{nullptr};

      for(auto& d : interface.descr_list)
      {
         config_descr.total_length += d.getLength();

         switch(d.getType())
         {
         case TYPE_INTERFACE:
            interface_descr = (InterfaceDescr*)&d;
            interface_descr->num_endpoints = 0;
            interface_descr->number        = config_descr.num_interfaces++;
            break;

         case TYPE_ENDPOINT:
            if (interface_descr != nullptr)
            {
               interface_descr->num_endpoints++;
               EndPointDescr* endpoint_descr = (EndPointDescr*)&d;
               endpoint_descr->addr = (endpoint_descr->addr & 0xF0) | ep_addr;

               // TODO this should move to set configuration time
               buffer_handler[ep_addr] = &interface;

               ep_addr++;
            }
            break;

         default:
            break;
         }
      }

#if 0
      // Debug
      for(auto& d : interface.descr_list)
      {
         char ch;

         switch(d.getType())
         {
         case TYPE_INTERFACE:    ch = 'I'; break;
         case TYPE_CS_INTERFACE: ch = 'i'; break;
         case TYPE_ENDPOINT:     ch = 'E'; break;
         case TYPE_CS_ENDPOINT:  ch = 'e'; break;
         default:                ch = '?'; break;
         }

         putchar(ch);

         const uint8_t* r = d.getRaw();
         for(unsigned i = 0; i < r[0]; ++i)
         {
            printf(" %02x", r[i]);
         }
         printf("\n");
      }
#endif
   }
}

} // namespace USB
