//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Widget.h"

namespace GUI {

class Col : public Widget
{
public:
   Col(Widget* parent_ = nullptr, unsigned gap_ = 0)
      : Widget(parent_)
   {
      setCol();
      setBorderAndGap(gap_);
      setShrink();
   }
};

} // namespace GUI
