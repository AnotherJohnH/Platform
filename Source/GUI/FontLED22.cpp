//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

// Bitmap font definition

#include "GUI/Font/LED.h"

namespace GUI {

static const uint8_t font_led22_data[] =
{
   0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C,
   0x60, 0x0C, 0x60, 0x0D, 0x00, 0x01, 0x00, 0x00, 0x60, 0x0D, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C,
   0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0,
   0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x01, 0x60, 0x00, 0x60, 0x00, 0x1F, 0xF0,
   0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C,
   0x60, 0x0C, 0x60, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0,
   0x1F, 0xF0, 0x60, 0x01, 0x60, 0x01, 0x60, 0x00, 0x60, 0x01, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00,
   0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x60, 0x00,
   0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x1F, 0xF0, 0x1F, 0xF0,
   0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x1F, 0xF0,
   0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C,
   0x60, 0x0C, 0x60, 0x0C, 0x1F, 0xF1, 0x1F, 0xF0, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0D, 0x60, 0x0C,
   0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x1F, 0xF0,
   0x1F, 0xF0, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C,
   0x1F, 0xF0, 0x1F, 0xF1, 0x00, 0x0D, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
   0x00, 0x0C, 0x1F, 0xF0, 0x1F, 0xF0, 0x00, 0x00
};

const Font font_led22 = {{15, 22}, 0x30, 0x39, 1, font_led22_data};

} // namespace GUI
