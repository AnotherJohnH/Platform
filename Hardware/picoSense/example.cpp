//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "GUI/Font/Teletext.h"
#include "STB/Colour.h"

#include "Hardware/picoSense/Config.h"
#include "Hardware/FilePortal.h"

static hw::FilePortal file_portal{"picoSense",
                          "https://github.com/AnotherJohnH/Hardware/blob/main/picoSense"};

inline const STB::Colour WHITE = STB::RGB(0xC0, 0xC0, 0xC0);

static hw::Led             led;
static hw::Buttons         buttons{/* enable_irq */ true};
static hw::Display         display{};
static hw::Display::Canvas canvas{};
static hw::UsbFile         usb{0x91C0, "picoSense", file_portal};
static hw::TempSense       sensor;

extern "C" void IRQ_IO_BANK0() { buttons.irq(); }
extern "C" void IRQ_USBCTRL()  { usb.irq(); }

int main()
{
   file_portal.addREADME("picoSense");

   display.setBrightness(0xC0);

   led = false;

   canvas.clear(WHITE);
   canvas.drawRect(STB::BLACK, 0, 0, canvas.getWidth() - 1, canvas.getHeight() - 1);
   canvas.drawText(STB::BLACK, WHITE, 10, 10, &GUI::font_teletext18, "Hello, World!");
   canvas.drawText(STB::BLACK, WHITE, 10, 30, &GUI::font_teletext9,
                   "ABCDEFGHIJKLMNOPOQRSTUVWXYZ");
   canvas.drawText(STB::BLACK, WHITE, 10, 40, &GUI::font_teletext9,
                   "abcdefghijklmnopoqrstuvwxyz");
   canvas.drawText(STB::BLACK, WHITE, 10, 50, &GUI::font_teletext9,  "0123456789");
   canvas.refresh();

   sensor.start();

   while(true)
   {
      unsigned index;
      bool     down;

      if (buttons.popEvent(index, down))
      {
         led = not led;

         if (down)
         {
            signed value = sensor.read() * 10 / 256;

            const char DEGREE_SYMBOL = 0x7F;

            char text[32];
            snprintf(text, sizeof(text), "%d.%u%cC\n",
                     value / 10, value % 10, DEGREE_SYMBOL);

            canvas.clear(WHITE);
            canvas.drawText(STB::BLACK, WHITE, 10, 10, &GUI::font_teletext18, text);
            canvas.refresh();
         }
      }

      usleep(50000);
   }

   return 0;
}
