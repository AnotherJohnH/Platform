//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Row.h"

namespace GUI {

class MenuBar : public GUI::Row
{
public:
   MenuBar(Widget* parent_) : GUI::Row(parent_, /* gap */ 4) {}
};

} // namespace GUI
