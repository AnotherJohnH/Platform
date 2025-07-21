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
