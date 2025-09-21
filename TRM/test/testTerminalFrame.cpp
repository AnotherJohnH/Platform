//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "TRM/FrameDevice.h"


int main(int argc, char *argv[])
{
   TRM::FrameDevice<512, 512> frame("TRM::Frame test");

   frame.write("Hello, world!\n\n>");

   frame.ioctl(TRM::Device::IOCTL_TERM_ECHO, 0);

   char ch;
   while(frame.read(ch) >= 0)
   {
      frame.write(ch);

      if (ch == '\n')
      {
         frame.write('>');
      }
   }

   return 0;
}
