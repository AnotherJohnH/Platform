//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "TRM/FrameDevice.h"
#include "TRM/Curses.h"


int main(int argc, char *argv[])
{
   TRM::FrameDevice<512, 512> frame("TRM::Frame test");
   TRM::Curses win(&frame);

   win.clear();
   win.addstr("Hello, world!\n");

   win.addstr("\033[30mBLACK\n");
   win.addstr("\033[31mRED\n");
   win.addstr("\033[32mGREEN\n");
   win.addstr("\033[33mYELLOW\n");
   win.addstr("\033[34mBLUE\n");
   win.addstr("\033[35mMAGENTA\n");
   win.addstr("\033[36mCYAN\n");
   win.addstr("\033[37mWHITE\n");
   win.addstr("\033[38;5;240mEXTEND\n");
   win.addstr("\033[40mBLACK\n");
   win.addstr("\033[41mRED\n");
   win.addstr("\033[42mGREEN\n");
   win.addstr("\033[43mYELLOW\n");
   win.addstr("\033[44mBLUE\n");
   win.addstr("\033[45mMAGENTA\n");
   win.addstr("\033[46mCYAN\n");
   win.addstr("\033[47mWHITE\n");

   win.attrset(0);

   win.attron(TRM::A_BOLD);
   win.addstr("bold      ");
   win.attroff(TRM::A_BOLD);
   win.addstr("  off\n");

   win.attron(TRM::A_DIM);
   win.addstr("faint     ");
   win.attroff(TRM::A_DIM);
   win.addstr("  off\n");

   win.attron(TRM::A_ITALIC);
   win.addstr("italic    ");
   win.attroff(TRM::A_ITALIC);
   win.addstr("  off\n");

   win.attron(TRM::A_REVERSE);
   win.addstr("invert    ");
   win.attroff(TRM::A_REVERSE);
   win.addstr("  off\n");

   win.attron(TRM::A_UNDERLINE);
   win.addstr("underline ");
   win.attroff(TRM::A_UNDERLINE);
   win.addstr("  off\n");

   while(win.getch() != -1) {}

   return 0;
}
