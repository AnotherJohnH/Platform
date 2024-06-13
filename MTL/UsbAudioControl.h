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
// \brief USB Audio Control types

#pragma once

#include "UsbTypes.h"

namespace USB {

namespace AC {

static const uint8_t SUB_CLASS = 0x01;

//------------------------------------------------------------------------------
// Class-specific descriptors

struct HeaderInterfaceDescr
{
   HeaderInterfaceDescr() = default;

   uint8_t  length{sizeof(HeaderInterfaceDescr)};
   uint8_t  type{CS_INTERFACE};
   uint8_t  sub_type{1};
   uint16_t version_bcd{0x100};
   uint16_t total_length{sizeof(HeaderInterfaceDescr)};
   uint8_t  in_collection{1};  // Number of streaming interfaces
   uint8_t  a_interface_nr{1}; // MIDI streaming interface 1 belongs to this AudioControl interface

} __attribute__((__packed__));


//------------------------------------------------------------------------------
// Management

class HeaderInterface : public CSInterface
{
public:
   HeaderInterface(Interface& interface_)
      : CSInterface(interface_, descr)
   {
   }

   HeaderInterfaceDescr descr{};
};

} // namespace AC

} // namespace USB
