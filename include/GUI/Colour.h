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

#ifndef GUI_COLOUR_H
#define GUI_COLOUR_H

#include <cstdint>

namespace GUI {


//! Packed pixel colour type
using Colour = uint32_t;


//! Least significant bit for fields in the packed pixel colour type Colour
const unsigned ALP_LSB = 24;
const unsigned RED_LSB = 16;
const unsigned GRN_LSB = 8;
const unsigned BLU_LSB = 0;


//! Compute a Colour value from red, green, blu and alpha components
static constexpr Colour RGBA(uint8_t red, uint8_t grn, uint8_t blu, uint8_t alpha)
{
   return (alpha << ALP_LSB) | (red << RED_LSB) | (grn << GRN_LSB) | (blu << BLU_LSB);
}


//! Compute a Colour value from red, green, blu components
static constexpr Colour RGB(uint8_t red, uint8_t grn, uint8_t blu)
{
   return RGBA(red, grn, blu, 0x00);
}


//! Compute a Colour value for a grey level
static constexpr Colour GREY(uint8_t level) { return RGB(level, level, level); }


//! Helper class to extract fields from packed colour type Colour
struct ColourDecode
{
   Colour colour;

   ColourDecode(Colour colour_)
      : colour(colour_)
   {
   }

   uint8_t alp() const { return colour >> ALP_LSB; }
   uint8_t red() const { return colour >> RED_LSB; }
   uint8_t grn() const { return colour >> GRN_LSB; }
   uint8_t blu() const { return colour >> BLU_LSB; }
};


// Some pre-defined packed colour values

const Colour BLACK   = RGB(0x00, 0x00, 0x00);
const Colour BLUE    = RGB(0x00, 0x00, 0xFF);
const Colour GREEN   = RGB(0x00, 0xFF, 0x00);
const Colour CYAN    = RGB(0x00, 0xFF, 0xFF);
const Colour RED     = RGB(0xFF, 0x00, 0x00);
const Colour MAGENTA = RGB(0xFF, 0x00, 0xFF);
const Colour YELLOW  = RGB(0xFF, 0xFF, 0x00);
const Colour WHITE   = RGB(0xFF, 0xFF, 0xFF);

const Colour BACKGROUND = WHITE;
const Colour FOREGROUND = BLACK;
const Colour DARK       = GREY(0x60);
const Colour SHADOW     = GREY(0x80);
const Colour FACE       = GREY(0xB0);
const Colour LIGHT      = GREY(0xD0);
const Colour HILIGHT    = GREY(0xF0);
const Colour HIDDEN     = RGBA(0x00, 0x00, 0x00, 0xFF);


} // namespace GUI

#endif
