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

#include <cstdint>

namespace USB {


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

enum DescrType
{
   DEVICE    = 0x1,
   CONFIG    = 0x2,
   STRING    = 0x3,
   INTERFACE = 0x4,
   ENDPOINT  = 0x5
};


struct DeviceDescr
{
   DeviceDescr() = default;

   uint8_t  length{sizeof(DeviceDescr)};
   uint8_t  type{DEVICE};
   uint16_t usb_bcd{0x110};         // USB 1.1
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

   uint8_t  length{sizeof(ConfigDescr)};
   uint8_t  type{CONFIG};
   uint16_t total_length{sizeof(ConfigDescr)};
   uint8_t  num_interfaces{0};
   uint8_t  value{1};
   uint8_t  name_idx{0};
   uint8_t  attributes{0b11000000};
   uint8_t  max_power{50};

} __attribute__((__packed__));


struct InterfaceDescr
{
   InterfaceDescr() = default;

   uint8_t  length{sizeof(InterfaceDescr)};
   uint8_t  type{INTERFACE};
   uint8_t  number{0};
   uint8_t  alternate_setting{0};
   uint8_t  num_endpoints{0};
   uint8_t  clas{0xFF};
   uint8_t  sub_class{0};
   uint8_t  protocol{0};
   uint8_t  name_idx{0};
};


struct EndPointDescr
{
   enum Dir  { OUT = 0b00000000, IN = 0b10000000 };
   enum Type { CONTROL = 0b00, ISOCHRONOUS = 0b01, BULK = 0b10, INTERRUPT = 0b11 };

   EndPointDescr(unsigned addr_, Dir dir_, Type type_)
   {
      addr = dir_ | addr_;
      attr = type_;
   }

   uint8_t  length{sizeof(EndPointDescr)};
   uint8_t  type{ENDPOINT};
   uint8_t  addr{0};
   uint8_t  attr{0};
   uint16_t max_packet_size{64};
   uint8_t  interval{0};

} __attribute__((__packed__));


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
class EndPoint;

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

   Config* addConfig(Config* config_)
   {
      descr.num_configs++;

      Config* next = config_list;
      config_list  = config_;
      return next;
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

   void setClassAndProtocol(uint8_t class_, uint8_t sub_class_, uint8_t protocol_)
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

   DeviceDescr descr{};

private:
   Config*    config_list{nullptr};
   Config*    active_config{nullptr};
   uint8_t    string_idx{0};
   uint8_t    string_buffer[256];
   Interface* buffer_handler[16] = {};
};


class Config
{
public:
   Config(Device& device_)
   {
      device = &device_;
      next   = device->addConfig(this);

      descr.attributes = 0b11000000; // USB 1.0 Bus Powered, no remote wakeup
      descr.max_power  = 50;         // 100 mA
   }

   Interface* addInterface(Interface* interface_)
   {
      descr.num_interfaces++;
      descr.total_length += sizeof(InterfaceDescr);

      Interface* next = interface_list;
      interface_list  = interface_;
      return next;
   }

   void addEndPoint(unsigned ep_, Interface* handler_)
   {
      device->setBufferHandler(ep_, handler_);

      descr.total_length += sizeof(EndPointDescr);
   }

   uint8_t addString(const char* string_)
   {
      return device->addString(string_);
   }

   void setName(const char* name_) { descr.name_idx = addString(name_); }

   Interface* getFirstInterface() { return interface_list; }
   Config*    getNext() { return next; }

   ConfigDescr descr{};

private:
   Device*    device;
   Config*    next{nullptr};
   Interface* interface_list{nullptr};
};


class Interface
{
public:
   Interface(Config& config_)
   {
      config = &config_;
      next   = config->addInterface(this);

      descr.number = config->descr.num_interfaces - 1;
   }

   EndPoint* addEndPoint(EndPoint* endpoint_, uint8_t ep_)
   {
      descr.num_endpoints++;

      config->addEndPoint(ep_, this);

      EndPoint* next = endpoint_list;
      endpoint_list  = endpoint_;
      return next;
   }

   void setClassAndProtocol(uint8_t class_, uint8_t sub_class_, uint8_t protocol_)
   {
      descr.clas      = class_;
      descr.sub_class = sub_class_;
      descr.protocol  = protocol_;
   }

   void setName(const char* name_) { descr.name_idx = config->addString(name_); }

   EndPoint*  getFirstEndPoint() { return endpoint_list; }
   Interface* getNext() { return next; }

   //! Handle a buffer recieved
   virtual void buffRx(uint8_t ep_, const uint8_t* data_, unsigned len_) {}

   //! Handle a buffer sent
   virtual void buffTx(uint8_t ep_) {}

   InterfaceDescr descr{};

private:
   Config*    config;
   Interface* next{nullptr};
   EndPoint*  endpoint_list{nullptr};
};


class EndPoint
{
public:
   EndPoint(EndPointDescr::Dir dir_)
      : descr(0, dir_, EndPointDescr::CONTROL)
   {
   }

   EndPoint(Interface&          interface_,
            unsigned            addr_,
            EndPointDescr::Dir  dir_,
            EndPointDescr::Type type_)
      : descr(addr_, dir_, type_)
   {
      next = interface_.addEndPoint(this, addr_ & 0xF);
   }

   EndPoint* getNext() { return next; }

   EndPointDescr descr;

private:
   EndPoint* next{nullptr};
};


Config* Device::getConfig(uint8_t number_)
{
   for(Config* config = config_list; config; config->getNext())
   {
      if (config->descr.value == number_)
         return config;
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

}
