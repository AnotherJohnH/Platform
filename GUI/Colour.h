//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "STB/Colour.h"

namespace GUI {

// Some pre-defined packed colour values
const STB::Colour BACKGROUND = STB::WHITE;
const STB::Colour FOREGROUND = STB::BLACK;
const STB::Colour DARK       = STB::GREY(0x60);
const STB::Colour SHADOW     = STB::GREY(0x80);
const STB::Colour FACE       = STB::GREY(0xB0);
const STB::Colour LIGHT      = STB::GREY(0xD0);
const STB::Colour HILIGHT    = STB::GREY(0xF0);

//! Background and foreground colour pair
class ColourPair
{
public:
   void setForegroundColour(STB::Colour colour_) { fg_colour = colour_; }
   void setBackgroundColour(STB::Colour colour_) { bg_colour = colour_; }

protected:
   STB::Colour fg_colour{FOREGROUND};
   STB::Colour bg_colour{FACE};
};

} // namespace GUI
