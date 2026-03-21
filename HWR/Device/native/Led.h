//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Simulated LED interface

#pragma once

#include "GUI/Font/Lcd.h"

#include "Panel.h"

template <unsigned SCALE = 2>
class Led
{
public:
   Led() = default;

   bool operator=(bool state_)
   {
      if (state != state_)
      {
         redraw = true;
         state = state_;
      }

      if (redraw)
      {
         redraw = false;
         draw();
      }

      panel.eventPoll();

      return state;
   }

   operator bool() const { return state; }

private:
   void draw()
   {
      panel.clear(COL_PCB);
      panel.fillRect(COL_LED, 4, 4, 16, 10);
      panel.fillRect(state ? COL_ON : COL_OFF, 6, 4, 14, 10);
      panel.drawText(state ? COL_TXT_ON : COL_TXT, 0x000000, 2, 15, &GUI::font_lcd, "LED");

      panel.refresh();
   }

   static const STB::Colour COL_PCB = STB::RGB(0x00, 0x50, 0x00);
   static const STB::Colour COL_LED = STB::RGB(0xC0, 0xC0, 0xC0);
   static const STB::Colour COL_OFF = STB::RGB(0xA0, 0xA0, 0x40);
   static const STB::Colour COL_ON  = STB::RGB(0x00, 0xFF, 0x00);
   static const STB::Colour COL_TXT = STB::RGB(0xD0, 0xD0, 0xD0);
   static const STB::Colour COL_TXT_ON = STB::RGB(0xD0, 0xF0, 0xD0);

   static const unsigned WIDTH  = 20;
   static const unsigned HEIGHT = 26;

   Panel<WIDTH,HEIGHT,SCALE> panel{};

   bool redraw{true};
   bool state{false};
};
