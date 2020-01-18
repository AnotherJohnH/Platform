//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

//! \file  Periph.h
//! \brief Memory mapped peripheral helper

#ifndef MTL_PERIPH_H
#define MTL_PERIPH_H

#include <cstdint>

#include "MTL/Register.h"

#define  REG_TYPE(OFFSET, TYPE, NAME)  \
    struct { uint8_t pad_##NAME[OFFSET]; TYPE NAME; }

#define  REG_TYPE_ARRAY(OFFSET, TYPE, NAME, SIZE)  \
    struct { uint8_t pad_##NAME[OFFSET]; TYPE NAME[SIZE]; }

#define  REG(OFFSET, NAME) \
    REG_TYPE(OFFSET, Register<uint32_t>, NAME)

#define  REG_ARRAY(OFFSET, NAME, SIZE)  \
    REG_TYPE_ARRAY(OFFSET, Register<uint32_t>, NAME, SIZE)

namespace MTL {

//! Base class for memory mapped peripherals
template <typename REG_TYPE,
          uint32_t BASE_ADDR,
          unsigned INSTANCE=0,
          uint32_t SIZE = 0x1000>
class Periph
{
protected:
   const uint32_t PERIPH_ADDR = BASE_ADDR + INSTANCE*SIZE;

public:
   volatile REG_TYPE* const reg = (volatile REG_TYPE*)(PERIPH_ADDR);
};

} // namespace MTL

#endif // MTL_PERIPH_H
