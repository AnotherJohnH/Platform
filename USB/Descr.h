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
// \brief USB descriptors

#pragma once

#include <cstdint>

#include "STB/List.h"

namespace USB {

// Device and interface class codes
enum Class : uint8_t
{
   CLASS_INFO_IN_IFC  = 0x00,
   CLASS_AUDIO        = 0x01,
   CLASS_COMMS        = 0x02,
   CLASS_HID          = 0x03,
   CLASS_MASS_STORAGE = 0x08,
   CLASS_VENDOR_SPEC  = 0xFF
};

// Got to use something!
static const uint16_t DUMMY_VENDOR_ID = 0xC0DE;

// Descriptor type codes
enum DescrType : uint8_t
{
    TYPE_DEVICE       = 0x01,
    TYPE_CONFIG       = 0x02,
    TYPE_STRING       = 0x03,
    TYPE_INTERFACE    = 0x04,
    TYPE_ENDPOINT     = 0x05,
    TYPE_CS_INTERFACE = 0x24,
    TYPE_CS_ENDPOINT  = 0x25
};


//! Base class for list element descriptors
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
   DescrType getType() const { return DescrType(raw[1]); }

   void* getImpl() const { return impl; }

private:
   const uint8_t* raw{};
   void*          impl{nullptr};
};


struct DeviceDescr
{
   DeviceDescr() = default;

   uint8_t  length{0x12};
   uint8_t  type{TYPE_DEVICE};
   uint16_t usb_bcd{0x200};          // USB 2.0
   uint8_t  clas{CLASS_INFO_IN_IFC};
   uint8_t  sub_class{0};
   uint8_t  protocol{0};
   uint8_t  max_packet_size0{64};
   uint16_t vendor_id{DUMMY_VENDOR_ID};
   uint16_t product_id{0x0001};
   uint16_t device_bcd{0x0100};      // 1.0
   uint8_t  vendor_idx{0};
   uint8_t  product_idx{0};
   uint8_t  serial_num_idx{0};
   uint8_t  num_configs{0};

} __attribute__((__packed__));


struct ConfigDescr
{
   ConfigDescr() = default;

   uint8_t  length{9};
   uint8_t  type{TYPE_CONFIG};
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
   uint8_t type{TYPE_INTERFACE};
   uint8_t number{0};
   uint8_t alternate_setting{0};
   uint8_t num_endpoints{0};
   uint8_t clas{CLASS_VENDOR_SPEC};
   uint8_t sub_class{0};
   uint8_t protocol{0};
   uint8_t name_idx{0};

} __attribute__((__packed__));


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
   uint8_t  type{TYPE_ENDPOINT};
   uint8_t  addr{0};
   uint8_t  attr{0};
   uint16_t max_packet_size{64};
   uint8_t  interval{0};

} __attribute__((__packed__));


} // namespace USB
