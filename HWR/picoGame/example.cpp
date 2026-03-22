//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "GUI/Font/Teletext.h"
#include "STB/Colour.h"

#include "HWR/picoGame/Config.h"
#include "HWR/FilePortal.h"

static HWR::FilePortal file_portal{"picoGame",
                          "https://github.com/SloeComputers/HWR/blob/main/picoGame"};

static HWR::Led             led;
static HWR::Buttons         buttons{/* enable_irq */ true};
static HWR::Display         display{};
static HWR::Display::Canvas canvas{};
static HWR::UsbFile         usb{0x91C0, "picoGame", file_portal};

extern "C" void IRQ_IO_BANK0() { buttons.irq(); }
extern "C" void IRQ_USBCTRL()  { usb.irq(); }

int main()
{
   file_portal.addREADME("picoGame");

   display.setBrightness(0xC0);

   led = false;

   canvas.clear(STB::BLACK);
   canvas.drawText(STB::WHITE, STB::BLACK, 10, 10, &GUI::font_teletext18, "Hello, world!");
   canvas.refresh();

   while(true)
   {
      unsigned index;
      bool     down;

      if (buttons.popEvent(index, down))
      {
         led = not led;
      }

      usleep(50000);
   }

   return 0;
}
