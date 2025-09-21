//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Window.h"

namespace GUI {

class PopUpWindow : public Window
{
public:
   PopUpWindow(Window*     parent_,
               const char* title_,
               unsigned    width_ = 0,
               unsigned    height_ = 0,
               uint32_t    flags_ = 0)
      : Window(title_, width_, height_, parent_->getDefaultFont(), flags_)
   {
      parent_->addChild(this);
   }
};

} // namespace GUI
