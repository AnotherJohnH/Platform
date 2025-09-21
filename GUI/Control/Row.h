//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Widget.h"

namespace GUI {

//! Horizontal widget container
class Row : public Widget
{
public:
   Row(Widget* parent_ = nullptr, unsigned gap_ = 0)
      : Widget(parent_)
   {
      setRow();
      setBorderAndGap(gap_);
      setShrink();
   }
};

} // namespace GUI
