//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Button.h"
#include "Symbol.h"

namespace GUI {

class SymbolButton : public Button
{
public:
   SymbolButton(Widget* parent_, unsigned code_, Symbol::Type type_)
      : Button(parent_, code_)
      , symbol(this, type_)
   {
   }

private:
   Symbol symbol;
};

} // namespace GUI
