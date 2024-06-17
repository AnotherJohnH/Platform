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

static const uint8_t SUB_CLASS = 0x03; //!< MIDISTREAMING audio interface

//------------------------------------------------------------------------------
// Class-specific descriptors

// Interface sub-types
static const uint8_t HEADER   = 0x1;
static const uint8_t IN_JACK  = 0x2;
static const uint8_t OUT_JACK = 0x3;
static const uint8_t ELEMENT  = 0x4;

// Interface jack types
static const uint8_t EMBEDDED = 0x1;
static const uint8_t EXTERNAL = 0x2;

// EndPoint sub-types
static const uint8_t GENERAL  = 0x1;


struct HeaderDescr : public Descr
{
   HeaderDescr(List& list_)
      : Descr(list_, length)
   {}

   uint8_t  length{7};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{HEADER};
   uint16_t version_bcd{0x100};    // MIDI Streaming sub-class release 1.00
   uint16_t total_length{0};       // XXX not clear what this is the total of

} __attribute__((__packed__));


struct JackInDescr : public Descr
{
   JackInDescr(List& list_, uint8_t jack_type_, uint8_t jack_id_)
      : Descr(list_, length)
      , jack_type(jack_type_)
      , jack_id(jack_id_)
   {}

   uint8_t length{6};
   uint8_t type{CS_INTERFACE};
   uint8_t sub_type{IN_JACK};
   uint8_t jack_type;
   uint8_t jack_id;
   uint8_t jack_idx{0};

} __attribute__((__packed__));


template <unsigned N>
struct JackOutDescr : public Descr
{
   JackOutDescr(List& list_, uint8_t jack_type_, uint8_t jack_id_)
      : Descr(list_, length)
      , jack_type(jack_type_)
      , jack_id(jack_id_)
   {}

   uint8_t length{7 + 2 * N};
   uint8_t type{CS_INTERFACE};
   uint8_t sub_type{OUT_JACK};
   uint8_t jack_type;
   uint8_t jack_id;
   uint8_t nr_input_pins{N};

   struct Source
   {
      uint8_t id{};
      uint8_t pin{1};

   } source[N];

   uint8_t jack_idx{0};

} __attribute__((__packed__));


template <unsigned N>
struct CSEndPointDescr : public Descr
{
   CSEndPointDescr(List& list_, uint8_t jack_id_)
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


struct EndPointDescr : public Descr
{
   EndPointDescr(List& list_, uint8_t dir_, uint8_t type_)
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
