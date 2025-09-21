//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Button.h
//! \brief microbit module buttons

#ifndef MTL_MICROBIT_V2_BUTTON_H
#define MTL_MICROBIT_V2_BUTTON_H

#include "MTL/Digital.h"

namespace MTL {

enum ButtonId
{
   BTN_A = PIN_BTN_A,
   BTN_B = PIN_BTN_B
};

template <ButtonId PIN>
class Button
{
public:
   operator bool() const { return not pin; }

private:
   Digital::In<PIN> pin;
};

} // namespace MTL

#endif // MTL_MICROBIT_V2_BUTTON_H
