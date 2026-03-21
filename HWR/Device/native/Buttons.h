//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Simulated LED interface

#pragma once

#include "GUI/Font/Lcd.h"
#include "STB/Keypad.h"

#include "Panel.h"

template <unsigned NUM_BUTTONS, unsigned SCALE = 1>
class Buttons
   : public Rack::EventHandler
   , public STB::Keypad<NUM_BUTTONS>
{
public:
   Buttons(bool enable_irq_ = false)
      : STB::Keypad<NUM_BUTTONS>(/* manual_scan */ true)
   {
   }

   void irq() {}

private:
   void keypadScan() override
   {
      panel.eventPoll();

      draw();
   }

   void draw()
   {
      panel.clear(COL_BGND);

      for(unsigned i = 0; i < NUM_BUTTONS; ++i)
      {
         unsigned x = BUTTON_WIDTH / 2;
         unsigned y = i * BUTTON_HEIGHT + BUTTON_HEIGHT / 2;

         panel.fillRect(0x000000, x - 13, y - 13, x + 13, y + 13);
         panel.fillRect(STB::RGB(0xC0, 0xC0, 0x00), x - 18, y - 11, x - 13, y - 5);
         panel.fillRect(STB::RGB(0xC0, 0xC0, 0x00), x - 18, y +  5, x - 13, y + 11);
         panel.fillRect(STB::RGB(0xC0, 0xC0, 0x00), x + 13, y - 11, x + 18, y - 5);
         panel.fillRect(STB::RGB(0xC0, 0xC0, 0x00), x + 13, y +  5, x + 18, y + 11);
         panel.fillCircle(STB::RGB(0x40, 0xC0, 0xFF), x, y, 8);
         panel.drawChar(STB::RGB(0xFF, 0xFF, 0xFF), 0, x - 2, y - 3, &GUI::font_lcd, '1' + i);
         panel.drawLine(0x000000, 0, y - 26, WIDTH, y - 26);
      }

      panel.refresh();
   }

   void eventClick(unsigned x_, unsigned y_, bool down_) override
   {
      unsigned index = y_ / BUTTON_HEIGHT;

      this->pushEvent(index, down_);
   }

   void eventKey(unsigned code_, bool down_) override
   {
      switch(code_)
      {
      case 'z':       this->pushEvent(0, down_); break;
      case ' ':       this->pushEvent(1, down_); break;
      case 'x':       this->pushEvent(2, down_); break;
      case PLT::UP:   this->pushEvent(3, down_); break;
      case PLT::DOWN: this->pushEvent(4, down_); break;
      }
   }

   static const STB::Colour COL_BGND = STB::RGB(0xE0, 0xE0, 0xE0);

   static const unsigned BUTTON_HEIGHT = 52;
   static const unsigned BUTTON_WIDTH  = 40;

   static const unsigned WIDTH  = BUTTON_WIDTH;
   static const unsigned HEIGHT = BUTTON_HEIGHT * NUM_BUTTONS;

   Panel<WIDTH,HEIGHT,SCALE> panel{this};
};
