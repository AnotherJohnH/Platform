//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

// Basic shared types

#ifndef GUI_TYPES_H
#define GUI_TYPES_H

#include <cstdint>

#include "MTH/Vector2.h"

namespace GUI {

using Colour = uint32_t;

const Colour BACKGROUND = 0x00FFFFFF;
const Colour FOREGROUND = 0x00000000;
const Colour DARK       = 0x00606060;
const Colour SHADOW     = 0x00808080;
const Colour FACE       = 0x00B0B0B0;
const Colour LIGHT      = 0x00D0D0D0;
const Colour HILIGHT    = 0x00F0F0F0;

const Colour BLACK      = 0x00000000;
const Colour BLUE       = 0x000000FF;
const Colour GREEN      = 0x0000FF00;
const Colour CYAN       = 0x0000FFFF;
const Colour RED        = 0x00FF0000;
const Colour MAGENTA    = 0x00FF00FF;
const Colour YELLOW     = 0x00FFFF00;
const Colour WHITE      = 0x00FFFFFF;

const Colour HIDDEN     = 0xFF000000;


union ColourEncDec
{
   ColourEncDec(Colour colour_)
      : colour(colour_)
   {}

   ColourEncDec(uint8_t red_, uint8_t grn_, uint8_t blu_)
      : blu(blu_), grn(grn_), red(red_)
   {}

   Colour  colour;

   // XXX Little endian processor assumed
   struct
   {
      uint8_t  blu;
      uint8_t  grn;
      uint8_t  red;
      uint8_t  alp;
   };
};


using Vector = MTH::Vector2<unsigned>;


// Window flags
const uint32_t RESIZABLE   = 1<<0;
const uint32_t NO_BORDER   = 1<<1;
const uint32_t FULL_SCREEN = 1<<2;


} // namespace GUI

#endif
