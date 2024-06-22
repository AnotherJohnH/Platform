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

class Config;

class Device
{
public:
   Device()
   {
      // Set English (US) as the language descriptor
      addString("\x09\x04", 2);
   }

   void addConfig(Config* config_)
   {
      descr.num_configs++;
      config_list.push_back(config_);
   }

   //! Allocate a StringDescr for a string \return index
   uint8_t addString(const char* string_, size_t length = 0)
   {
      uint8_t  idx    = string_idx;
      uint8_t* buffer = getString(idx);

      if (length == 0)
         length = strlen(string_);

      buffer[0] = length;
      memcpy(buffer + 1, string_, length);
      string_idx += 1 + length;

      return idx;
   }

   void setClassAndProtocol(uint8_t class_, uint8_t sub_class_, uint8_t protocol_ = 0)
   {
      descr.clas      = class_;
      descr.sub_class = sub_class_;
      descr.protocol  = protocol_;
   }

   void setVendor(const char* vendor_name_)
   {
      descr.vendor_idx = addString(vendor_name_);
   }

   void setProduct(uint16_t    product_id_,
                   uint16_t    bcd_version_,
                   const char* product_name_)
   {
      descr.product_id  = product_id_;
      descr.device_bcd  = bcd_version_;
      descr.product_idx = addString(product_name_);
   }

   void setSerialNumber(const char* serial_)
   {
      descr.serial_num_idx = addString(serial_);
   }

   void setBufferHandler(unsigned ep_, Interface* handler_)
   {
      buffer_handler[ep_] = handler_;
   }

   const DeviceDescr& getDescr() const { return descr; }

   uint8_t* getString(uint8_t idx_) { return &string_buffer[idx_]; }

   Config* getConfig(uint8_t number_);

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

private:
   DeviceDescr       descr{};
   STB::List<Config> config_list{};
   uint8_t           string_idx{0};
   uint8_t           string_buffer[256];
   Interface*        buffer_handler[16] = {};
};


// TODO split out into own header ... or maybe merge with Device
//      and only support one config
class Config : public STB::List<Config>::Element
{
public:
   Config(Device& device_)
   {
      device = &device_;
      device->addConfig(this);

      descr.attributes = 0b10100000; // USB 2.0 Bus Powered, no remote wakeup
      descr.max_power  = 50;         // 100 mA
   }

   uint8_t addString(const char* string_)
   {
      return device->addString(string_);
   }

   void setName(const char* name_) { descr.name_idx = addString(name_); }

   void linkDescriptors();

   ConfigDescr          descr{};
   STB::List<Interface> interface_list{};

private:
   Device* device;
};


Config* Device::getConfig(uint8_t number_)
{
   for(auto& cfg : config_list)
   {
      if (cfg.descr.value == number_)
         return &cfg;
   }

   return nullptr;
}


void Config::linkDescriptors()
{
   if (descr.total_length != 0)
      return;

   uint8_t ep_addr = 1;

   descr.num_interfaces = 0;
   descr.total_length   = descr.length;

   for(auto& interface : interface_list)
   {
      InterfaceDescr* interface_descr{nullptr};

      for(auto& d : interface.descr_list)
      {
         descr.total_length += d.getLength();

         switch(d.getType())
         {
         case TYPE_INTERFACE:
            interface_descr = (InterfaceDescr*)&d;
            interface_descr->num_endpoints = 0;
            interface_descr->number        = descr.num_interfaces++;
            break;

         case TYPE_ENDPOINT:
            if (interface_descr != nullptr)
            {
               interface_descr->num_endpoints++;
               EndPointDescr* endpoint_descr = (EndPointDescr*)&d;
               endpoint_descr->addr = (endpoint_descr->addr & 0xF0) | ep_addr;

               // TODO this should move to set configuration time
               device->setBufferHandler(ep_addr, &interface);

               ep_addr++;
            }
            break;

         default:
            break;
         }
      }

#if 0
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
