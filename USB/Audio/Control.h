//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief USB Audio Control types

#pragma once

#include "USB/Descr.h"

namespace USB {

namespace AC {

static const uint8_t SUB_CLASS = 0x01;

//------------------------------------------------------------------------------
// Class-specific descriptors

struct Header : public Descr
{
   Header(List& list_) : Descr(list_, length) {}

   uint8_t  length{9};
   uint8_t  type{TYPE_CS_INTERFACE};
   uint8_t  sub_type{1};
   uint16_t version_bcd{0x100};
   uint16_t total_length{9};
   uint8_t  in_collection{1};  // Number of streaming interfaces
   uint8_t  a_interface_nr{1}; // MIDI streaming interface 1 belongs to this AudioControl interface

} __attribute__((__packed__));

} // namespace AC

} // namespace USB
