//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Row.h"
#include "Text.h"
#include "TickBox.h"

namespace GUI {

class TextTickBox : public Row
{
public:
   TextTickBox(Widget* parent_, unsigned code_, const char* text_)
      : Row(parent_)
      , tick_box(this, code_)
      , text(this, text_)
   {
      gap        = 8;
      vert_align = Align::CENTER;
   }

   TickBox tick_box;
   Text    text;
};

} // namespace GUI
