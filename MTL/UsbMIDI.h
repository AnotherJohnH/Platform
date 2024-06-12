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
// \brief USB MIDI types

#pragma once

#include "UsbTypes.h"

namespace USB {

namespace MS {

static const uint8_t SUB_CLASS = 0x03;

//------------------------------------------------------------------------------
// Class-specific descriptors


// Interface sub-types
static const uint8_t EMBEDDED = 0x1;
static const uint8_t EXTERNAL = 0x2;

// EndPoint sub-types
static const uint8_t GENERAL  = 0x1;


struct HeaderInterfaceDescr
{
   HeaderInterfaceDescr() = default;

   uint8_t  length{sizeof(HeaderInterfaceDescr)};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{1};
   uint16_t version_bcd{0x100};
   uint16_t total_length{sizeof(HeaderInterfaceDescr)};

} __attribute__((__packed__));


struct InJackInterfaceDescr
{
   InJackInterfaceDescr() = default;

   uint8_t  length{sizeof(InJackInterfaceDescr)};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{2};
   uint8_t  jack_type{EMBEDDED};
   uint8_t  jack_id{};
   uint8_t  jack_idx{0};

} __attribute__((__packed__));


struct OutJackInterfaceDescr
{
   OutJackInterfaceDescr() = default;

   uint8_t  length{sizeof(OutJackInterfaceDescr)};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{3};
   uint8_t  jack_type{EMBEDDED};
   uint8_t  jack_id{};
   uint8_t  source_id{};
   uint8_t  soutrce_pin{};
   uint8_t  jack_idx{0};

} __attribute__((__packed__));


template <unsigned N>
struct EndPointDescr
{
   EndPointDescr(EndPoint& endpoint_)
   {
      endpoint_.setCSDescr(&length);
   }

   uint8_t length{sizeof(EndPointDescr<N>)};
   uint8_t type{CS_ENDPOINT};
   uint8_t sub_type{GENERAL}; 
   uint8_t num_emb_midi_jack{N};
   uint8_t assoc_jack_id[N] = {};

} __attribute__((__packed__));


//------------------------------------------------------------------------------
// Management

class HeaderInterface : public CSInterface
{
public:
   HeaderInterface(Interface& interface_)
      : CSInterface(interface_)
      , interface(&interface_)
   {
   }

   void addCSInterface(CSInterface* cs_interface_, size_t length_)
   {
      descr.total_length += length_;
      interface->addCSInterface(cs_interface_);
   }

   HeaderInterfaceDescr descr;

private:
   Interface* interface;
};

class InJackInterface : public CSInterface
{
public:
   InJackInterface(HeaderInterface& header_)
   {
      header_.addCSInterface(this, sizeof(descr));
   }

   InJackInterfaceDescr descr;
};

class OutJackInterface : public CSInterface
{
public:
   OutJackInterface(HeaderInterface& header_)
   {
      header_.addCSInterface(this, sizeof(descr));
   }

   OutJackInterfaceDescr descr;
};


} // namespace MS

} // namespace USB
