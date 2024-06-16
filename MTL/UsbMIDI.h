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


// Interface jack sub-types
static const uint8_t EMBEDDED = 0x1;
static const uint8_t EXTERNAL = 0x2;

// EndPoint sub-types
static const uint8_t GENERAL  = 0x1;


struct Header : public Descr
{
   Header(List& list_)
      : Descr(list_, length)
   {}

   uint8_t  length{7};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{1};
   uint16_t version_bcd{0x100};
   uint16_t total_length{0};

} __attribute__((__packed__));


struct JackIn : public Descr
{
   JackIn(List& list_, uint8_t jack_type_, uint8_t jack_id_)
      : Descr(list_, length)
      , jack_type(jack_type_)
      , jack_id(jack_id_)
   {}

   uint8_t  length{6};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{2};
   uint8_t  jack_type;
   uint8_t  jack_id;
   uint8_t  jack_idx{0};

} __attribute__((__packed__));


template <unsigned N>
struct JackOut : public Descr
{
   JackOut(List& list_, uint8_t jack_type_, uint8_t jack_id_)
      : Descr(list_, length)
      , jack_type(jack_type_)
      , jack_id(jack_id_)
   {}

   uint8_t  length{6 + 2 * N};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{3};
   uint8_t  jack_type;
   uint8_t  jack_id;

   struct Source
   {
      uint8_t id{};
      uint8_t pin{};

   } source[N];

   uint8_t  jack_idx{0};

} __attribute__((__packed__));


template <unsigned N>
struct CSEndPoint : public Descr
{
   CSEndPoint(List& list_, uint8_t jack_id_)
      : Descr(list_, length)
   {
      assoc_jack_id[0] = jack_id_;
   }

   uint8_t length{4 + N};
   uint8_t type{CS_ENDPOINT};
   uint8_t sub_type{GENERAL}; 
   uint8_t num_emb_midi_jack{N};
   uint8_t assoc_jack_id[N] = {};

} __attribute__((__packed__));


struct EndPoint : public Descr
{
   // Direction
   static const uint8_t OUT = 0b00000000;
   static const uint8_t IN  = 0b10000000;

   // Type
   static const uint8_t CONTROL     = 0b00;
   static const uint8_t ISOCHrONOUS = 0b00;
   static const uint8_t BULK        = 0b10;
   static const uint8_t INTERRUPt   = 0b11;

   EndPoint(List& list_, uint8_t dir_, uint8_t type_)
      : Descr(list_, length)
      , addr(dir_)
      , attr(uint8_t(type_))
   {
   }

   uint8_t  length{9};
   uint8_t  type{ENDPOINT};
   uint8_t  addr{0};
   uint8_t  attr{0};
   uint16_t max_packet_size{64};
   uint8_t  interval{0};
   uint8_t  refresh{0};
   uint8_t  sync_addr{0};

} __attribute__((__packed__));


} // namespace MS

} // namespace USB
