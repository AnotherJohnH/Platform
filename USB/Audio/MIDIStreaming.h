//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief USB MIDI types

#pragma once

#include "USB/Descr.h"

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
   uint8_t  type{TYPE_CS_INTERFACE};
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
   uint8_t type{TYPE_CS_INTERFACE};
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
   uint8_t type{TYPE_CS_INTERFACE};
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

#if defined(__GNUC__) && !defined(__clang__)
}; // XXX gcc 14 is not happy with packing this structure
   // we assume everything is still ok?
#else
} __attribute__((__packed__));
#endif


template <unsigned N>
struct CSEndPointDescr : public Descr
{
   CSEndPointDescr(List& list_, uint8_t jack_id_)
      : Descr(list_, length)
   {
      assoc_jack_id[0] = jack_id_;
   }

   uint8_t length{4 + N};
   uint8_t type{TYPE_CS_ENDPOINT};
   uint8_t sub_type{GENERAL}; 
   uint8_t num_emb_midi_jack{N};
   uint8_t assoc_jack_id[N] = {};

#if defined(__GNUC__) && !defined(__clang__)
}; // XXX gcc 14 is not happy with packing this structure
   // we assume everything is still ok?
#else
} __attribute__((__packed__));
#endif


struct EndPointDescr : public Descr
{
   EndPointDescr(List& list_, uint8_t dir_, uint8_t type_)
      : Descr(list_, length)
      , addr(dir_)
      , attr(uint8_t(type_))
   {
   }

   uint8_t  length{9};
   uint8_t  type{TYPE_ENDPOINT};
   uint8_t  addr{0};
   uint8_t  attr{0};
   uint16_t max_packet_size{64};
   uint8_t  interval{0};
   uint8_t  refresh{0};
   uint8_t  sync_addr{0};

} __attribute__((__packed__));


} // namespace MS

} // namespace USB
