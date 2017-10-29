//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------


#include <cstdio>

#include "TRM/Curses.h"
#include "TRM/Stdio.h"


int main(int argc, char *argv[])
{
   TRM::Stdio term("TRM::Stdio");
   TRM::Curses win(&term);

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
