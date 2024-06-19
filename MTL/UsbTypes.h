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
// \brief USB types

#pragma once

#include <cstdint>

#include "STB/List.h"

namespace USB {


static const uint8_t CLASS_AUDIO = 0x01;


enum class Request : uint8_t
{
   GET_STATUS     = 0x00,
   CLR_FEATURE    = 0x01,
   SET_FEATURE    = 0x02,
   SET_ADDRESS    = 0x05,
   GET_DESCRIPTOR = 0x06,
   SET_DESCRIPTOR = 0x07,
   GET_CONFIG     = 0x08,
   SET_CONFIG     = 0x09,
   GET_INTERFACE  = 0x0A,
   SET_INTERFACE  = 0x0B,
   SYNC_FRAME     = 0x0C
};

//! SETUP packet
struct SetupReq
{
   SetupReq() = default;

   unsigned isHostToDevice() const { return (type & 0b10000000) == 0; }
   unsigned getType()        const { return (type >> 5) & 0b11; }
   unsigned getRecipient()   const { return type & 0b11111; }

   void print()
   {
      printf(".type    = %02X\n", type);
      printf(".request = %02X\n", unsigned(request));
      printf(".value   = %04X\n", value);
      printf(".index   = %04X\n", index);
      printf(".length  = %04X\n", length);
   }

   uint8_t  type{};
   Request  request{};
   uint16_t value{};
   uint16_t index{};
   uint16_t length{};

} __attribute__((__packed__));


//------------------------------------------------------------------------------
// Descriptors

enum DescrType : uint8_t
{
   DEVICE       = 0x1,
   CONFIG       = 0x2,
   STRING       = 0x3,
   INTERFACE    = 0x4,
   ENDPOINT     = 0x5,
   CS_INTERFACE = 0x24,
   CS_ENDPOINT  = 0x25
};


//! Base class for descriptors
class Descr : public STB::List<Descr>::Element
{
public:
   using List = STB::List<Descr>;

   Descr() = default;

   Descr(const uint8_t& length_)
      : raw(&length_)
   {
   }

   Descr(List& list_, const uint8_t& length_)
      : raw(&length_)
   {
      list_.push_back(this);
   }

   //! Set option implementation pointer
   void setImpl(void* impl_) { impl = impl_; }

   //! Returns pointer to first byte in descriptor
   const uint8_t* getRaw() const { return raw; }

   //! Returns length of descriptor as a reference
   const uint8_t& getLength() const { return raw[0]; }

   //! Returns type of descriptor
   uint8_t getType() const { return raw[1]; }

   void* getImpl() const { return impl; }

private:
   const uint8_t* raw{};
   void*          impl{nullptr};
};


struct DeviceDescr
{
   DeviceDescr() = default;

   uint8_t  length{sizeof(DeviceDescr)};
   uint8_t  type{DEVICE};
   uint16_t usb_bcd{0x200};         // USB 2.0
   uint8_t  clas{0};
   uint8_t  sub_class{0};
   uint8_t  protocol{0};
   uint8_t  max_packet_size0{64};
   uint16_t vendor_id{0x0000};
   uint16_t product_id{0x0000};
   uint16_t device_bcd{0x0000};
   uint8_t  vendor_idx{0};
   uint8_t  product_idx{0};
   uint8_t  serial_num_idx{0};
   uint8_t  num_configs{0};

} __attribute__((__packed__));


struct ConfigDescr
{
   ConfigDescr() = default;

   uint8_t  length{9};
   uint8_t  type{CONFIG};
   uint16_t total_length{0};
   uint8_t  num_interfaces{0};
   uint8_t  value{1};
   uint8_t  name_idx{0};
   uint8_t  attributes{0b10100000};
   uint8_t  max_power{50};

} __attribute__((__packed__));



struct InterfaceDescr : public Descr
{
   InterfaceDescr() = default;

   InterfaceDescr(List& list_, uint8_t class_, uint8_t sub_class_, uint8_t protocol_ = 0)
      : Descr(list_, length)
      , clas(class_)
      , sub_class(sub_class_)
      , protocol(protocol_)
   {
   }

   uint8_t length{9};
   uint8_t type{INTERFACE};
   uint8_t number{0};
   uint8_t alternate_setting{0};
   uint8_t num_endpoints{0};
   uint8_t clas{0xFF};
   uint8_t sub_class{0};
   uint8_t protocol{0};
   uint8_t name_idx{0};

} __attribute__((__packed__));


// XXX seems a false positive with GCC 10.2.1
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow="

struct EndPointDescr : public Descr
{
   // Direction
   static const uint8_t OUT = 0b00000000;
   static const uint8_t IN  = 0b10000000;

   // Type
   static const uint8_t CONTROL     = 0b00;
   static const uint8_t ISOCHrONOUS = 0b00;
   static const uint8_t BULK        = 0b10;
   static const uint8_t INTERRUPt   = 0b11;
 
   EndPointDescr(List& list_, uint8_t dir_, uint8_t type_)
      : Descr(list_, length)
      , addr(dir_)
      , attr(uint8_t(type_))
   {
   }

   uint8_t  length{7};
   uint8_t  type{ENDPOINT};
   uint8_t  addr{0};
   uint8_t  attr{0};
   uint16_t max_packet_size{64};
   uint8_t  interval{0};

} __attribute__((__packed__));

#pragma GCC diagnostic pop


struct StringDescr
{
   uint8_t  length{sizeof(StringDescr)};
   uint8_t  type{STRING};
   char     text[1];

} __attribute__((__packed__));


//------------------------------------------------------------------------------
// Device Management

class Config;
class Interface;

class Device
{
public:
   Device(uint16_t vendor_id_,
          uint16_t product_id_,
          uint16_t device_bcd_ = 0x0100)
   {
      descr.vendor_id  = vendor_id_;
      descr.product_id = product_id_;
      descr.device_bcd = device_bcd_;

      // Set English (US) as StringDescr zero
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

   void setVendor(const char* vendor_) { descr.vendor_idx = addString(vendor_); }

   void setProduct(const char* product_) { descr.product_idx = addString(product_); }

   void setSerialNumber(const char* serial_) { descr.serial_num_idx = addString(serial_); }

   void setBufferHandler(unsigned ep_, Interface* handler_)
   {
      buffer_handler[ep_] = handler_;
   }

   uint8_t* getString(uint8_t idx_) { return &string_buffer[idx_]; }

   Config* getConfig(uint8_t number_);

   void handleBuffRx(uint8_t ep, const uint8_t* buffer, unsigned length);

   void handleBuffTx(uint8_t ep);

   DeviceDescr       descr{};
   STB::List<Config> config_list{};

private:
   uint8_t    string_idx{0};
   uint8_t    string_buffer[256];
   Interface* buffer_handler[16] = {};
};


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

   Device* device;
};


class Interface : public STB::List<Interface>::Element
{
public:
   Interface(Config& config_, uint8_t class_, uint8_t sub_class_, uint8_t protocol_ = 0)
      : descr(descr_list, class_, sub_class_, protocol_)
   {
      config_.interface_list.push_back(this);
   }

   virtual void configured() {}

   //! Handle a buffer recieved
   virtual void buffRx(uint8_t ep_, const uint8_t* data_, unsigned len_) {}

   //! Handle a buffer sent
   virtual void buffTx(uint8_t ep_) {}

   STB::List<Descr> descr_list{};

private:
   USB::InterfaceDescr descr;
};


class EndPoint
{
public:
   EndPoint() = default;

   EndPoint(Descr& descr_)
   {
      descr_.setImpl(this);
   }
};


Config* Device::getConfig(uint8_t number_)
{
   for(auto& config : config_list)
   {
      if (config.descr.value == number_)
         return &config;
   }

   return nullptr;
}

void Device::handleBuffRx(uint8_t ep, const uint8_t* buffer, unsigned length)
{
   if (buffer_handler[ep] != nullptr)
   {
      buffer_handler[ep]->buffRx(ep, buffer, length);
   }
}

void Device::handleBuffTx(uint8_t ep)
{
   if (buffer_handler[ep] != nullptr)
   {
      buffer_handler[ep]->buffTx(ep);
   }
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
         case INTERFACE:
            interface_descr = (InterfaceDescr*)&d;
            interface_descr->num_endpoints = 0;
            interface_descr->number        = descr.num_interfaces++;
            break;

         case ENDPOINT:
            if (interface_descr != nullptr)
            {
               interface_descr->num_endpoints++;
               EndPointDescr* endpoint_descr = (EndPointDescr*)&d;
               endpoint_descr->addr = (endpoint_descr->addr & 0xF0) | ep_addr;

               // TODO this should move to set configuration time
               device->setBufferHandler(ep_addr, &interface);

               // ep_addr++;
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
         case INTERFACE:    ch = 'I'; break;
         case CS_INTERFACE: ch = 'i'; break;
         case ENDPOINT:     ch = 'E'; break;
         case CS_ENDPOINT:  ch = 'e'; break;
         default:           ch = '?'; break;
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


}
