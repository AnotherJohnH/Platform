//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Button.h"
#include "Text.h"

namespace GUI {

class TextButton : public Button
{
public:
   TextButton() = default;

   TextButton(Widget* parent_, unsigned code_, const char* text_)
      : Button(parent_, code_)
      , text(this, text_)
   {
   }

   Text text{this};
};

} // namespace GUI
