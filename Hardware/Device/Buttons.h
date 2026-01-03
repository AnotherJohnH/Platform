//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Button interface

#pragma once

#if defined(HW_BUTTONS_GPIO)

#elif defined(HW_BADGER2040)

#elif defined(HW_TUFTY2040)

#elif defined(HW_NATIVE)

#include "native/Buttons.h"

#else

#include "MTL/chip/Gpio.h"

#endif

#include "STB/Keypad.h"

namespace hw {

#if defined(HW_BADGER2040)

class Buttons : public STB::Keypad<HW_BUTTONS>
{
public:
   Buttons(bool enable_irq_ = false)
      : STB::Keypad<HW_BUTTONS>(/* manual_scan */ not enable_irq_)
      , btn1(enable_irq_)
      , btn2(enable_irq_)
      , btn3(enable_irq_)
      , btn4(enable_irq_)
      , btn5(enable_irq_)
   {
      MTL::config.gpio(MTL::badger2040::PIN_SW_A,  "<BTN1 (A)");
      MTL::config.gpio(MTL::badger2040::PIN_SW_B,  "<BTN2 (B)");
      MTL::config.gpio(MTL::badger2040::PIN_SW_C,  "<BTN3 (C)");
      MTL::config.gpio(MTL::badger2040::PIN_SW_UP, "<BTN4 (UP)");
      MTL::config.gpio(MTL::badger2040::PIN_SW_DN, "<BTN5 (DN)");
   }

   using Keypad<5>::operator[];

   void irq()
   {
      btn1.ackIRQ();
      btn2.ackIRQ();
      btn3.ackIRQ();
      btn4.ackIRQ();
      btn5.ackIRQ();

      keypadScan();
   }

private:
   bool keypadSample(unsigned index_) const override
   {
      switch(index_)
      {
      case 0: return btn1;
      case 1: return btn2;
      case 2: return btn3;
      case 3: return btn4;
      case 4: return btn5;
      }

      return false;
   }

   MTL::badger2040::SwitchA  btn1;
   MTL::badger2040::SwitchB  btn2;
   MTL::badger2040::SwitchC  btn3;
   MTL::badger2040::SwitchUp btn4;
   MTL::badger2040::SwitchDn btn5;
};

#elif defined(HW_TUFTY2040)

class Buttons : public STB::Keypad<HW_BUTTONS>
{
public:
   Buttons(bool enable_irq_ = false)
      : STB::Keypad<HW_BUTTONS>(/* manual_scan */ not enable_irq_)
      , btn1(enable_irq_)
      , btn2(enable_irq_)
      , btn3(enable_irq_)
      , btn4(enable_irq_)
      , btn5(enable_irq_)
   {
      MTL::config.gpio(MTL::tufty2040::PIN_SW_A,  "<BTN1 (A)");
      MTL::config.gpio(MTL::tufty2040::PIN_SW_B,  "<BTN2 (B)");
      MTL::config.gpio(MTL::tufty2040::PIN_SW_C,  "<BTN3 (C)");
      MTL::config.gpio(MTL::tufty2040::PIN_SW_UP, "<BTN4 (UP)");
      MTL::config.gpio(MTL::tufty2040::PIN_SW_DN, "<BTN5 (DN)");
   }

   using Keypad<HW_BUTTONS>::operator[];

   void irq()
   {
      btn1.ackIRQ();
      btn2.ackIRQ();
      btn3.ackIRQ();
      btn4.ackIRQ();
      btn5.ackIRQ();

      keypadScan();
   }

private:
   bool keypadSample(unsigned index_) const override
   {
      switch(index_)
      {
      case 0: return btn1;
      case 1: return btn2;
      case 2: return btn3;
      case 3: return btn4;
      case 4: return btn5;
      }

      return false;
   }

   MTL::tufty2040::SwitchA  btn1;
   MTL::tufty2040::SwitchB  btn2;
   MTL::tufty2040::SwitchC  btn3;
   MTL::tufty2040::SwitchUp btn4;
   MTL::tufty2040::SwitchDn btn5;
};

#elif defined(HW_NATIVE)

using Buttons = ::Buttons<HW_BUTTONS>;

#elif defined(HW_BUTTONS)

class Buttons : public STB::Keypad<HW_BUTTONS>
{
public:
   Buttons(bool enable_irq_ = false)
      : STB::Keypad<HW_BUTTONS>(/* manual_scan */ not enable_irq_)
      , btns(enable_irq_)
   {
      MTL::config.gpio(HW_BUTTONS_PIN + 0, "<BTN1");
      if (HW_BUTTONS > 1) MTL::config.gpio(HW_BUTTONS_PIN + 1, "<BTN2");
      if (HW_BUTTONS > 2) MTL::config.gpio(HW_BUTTONS_PIN + 2, "<BTN3");
      if (HW_BUTTONS > 3) MTL::config.gpio(HW_BUTTONS_PIN + 3, "<BTN4");
      if (HW_BUTTONS > 4) MTL::config.gpio(HW_BUTTONS_PIN + 4, "<BTN5");
   }

   using Keypad<HW_BUTTONS>::operator[];

   void irq()
   {
      btns.ackIRQ();

      keypadScan();
   }

private:
   bool keypadSample(unsigned index_) const override
   {
      return (btns & (1 << index_)) != 0;
   }

   MTL::Gpio::In<HW_BUTTONS, HW_BUTTONS_PIN, MTL::PadsBank::PULL_DOWN, /* schmitt trigger */ true> btns;
};

#else

class Buttons : public STB::Keypad<5>
{
public:
   Buttons(bool enable_irq_ = false)
      : STB::Keypad<5>(/* manual_scan */ not enable_irq_)
   {}

   using Keypad<5>::operator[];

   void irq() {}

private:
   bool keypadSample(unsigned index_) const override { return false; }
};

#endif

} // namespace hw
