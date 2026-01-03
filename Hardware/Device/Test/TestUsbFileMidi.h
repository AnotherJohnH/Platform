//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "Test.h"
#include "Hardware/FilePortal.h"

namespace hw {

static hw::UsbFileMidi* usb_ptr{nullptr};

extern "C" void IRQ_USBCTRL()
{
   if (usb_ptr != nullptr)
   {
      usb_ptr->irq();
   }
}
   
inline NOINLINE void testUsbFileMidi(TestPhase phase_)
{  
   static hw::FilePortal file_portal{"HW_TEST",
                                     "https://github.com/AnotherJohnH/Hardware/"};
   
   static hw::UsbFileMidi usb{0x91C0, "test_hw", file_portal};
   
   switch(phase_)
   {
   case DECL:
      usb_ptr = &usb;
      break;

   case INFO:
      printf("USB: Mass storage device \"HW_TEST\" with a simple README\n" );
      printf("USB: Laggy log of MIDI messages on the console\n" );
      break;

   case START:
      usb.setDebug(true);
      file_portal.addREADME("Test hw::UsbFileMidi");
      break;

   case RUN:
      usb.tick();
      break;
   }
} 

} // namespace hw
