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


#include "TRM/Launcher.h"

#define  PROGRAM         "MyApp"
#define  DESCRIPTION     "A test"
#define  LINK            "https://github.com/AnotherJohnH/Platform"
#define  AUTHOR          "John D. Haughton"
#define  COPYRIGHT_YEAR  "2017"

class MyApp : public TRM::Launcher
{
private:
   // TODO remove this override if not required
   virtual void showExtraHelp() override
   {
      printf("Some extra help text.\n");
   }

   virtual int startTerminalLauncher(const char* file) override
   {
      // TODO Body of application goes here

      curses.addstr("Hello, world!\n");

      while(curses.getch() != -1) {}

      return 0;
   }

public:
   MyApp(int argc, const char* argv[])
      : TRM::Launcher(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR,
                      "<file>", "launcher.cfg")
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   MyApp(argc, argv);
}

