//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Widget.h"

namespace GUI {

//! Expanding container
class Expand : public Widget
{
public:
   Expand(Widget* parent_ = nullptr)
      : Widget(parent_)
   {
      setExpand();
   }
};

} // namespace GUI
