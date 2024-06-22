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

   uint8_t  type{};
   Request  request{};
   uint16_t value{};
   uint16_t index{};
   uint16_t length{};

} __attribute__((__packed__));


} // namespace USB
