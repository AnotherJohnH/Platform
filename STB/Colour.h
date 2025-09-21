//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef STB_COLOUR_H
#define STB_COLOUR_H

#include <cstdint>

namespace STB {


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
   return (Colour(alpha) << ALP_LSB) |
          (Colour(red) << RED_LSB) |
          (Colour(grn) << GRN_LSB) |
          (Colour(blu) << BLU_LSB);
}


//! Compute a Colour value from red, green, blu components
static constexpr Colour RGB(uint8_t red, uint8_t grn, uint8_t blu)
{
   return RGBA(red, grn, blu, 0x00);
}


//! Compute a Colour value for a grey level
static constexpr Colour GREY(uint8_t level) { return RGB(level, level, level); }


// Some pre-defined packed colour values
const Colour BLACK   = RGB(0x00, 0x00, 0x00);
const Colour BLUE    = RGB(0x00, 0x00, 0xFF);
const Colour GREEN   = RGB(0x00, 0xFF, 0x00);
const Colour CYAN    = RGB(0x00, 0xFF, 0xFF);
const Colour RED     = RGB(0xFF, 0x00, 0x00);
const Colour MAGENTA = RGB(0xFF, 0x00, 0xFF);
const Colour YELLOW  = RGB(0xFF, 0xFF, 0x00);
const Colour WHITE   = RGB(0xFF, 0xFF, 0xFF);


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

} // namespace STB

#endif // STB_COLOUR_H
