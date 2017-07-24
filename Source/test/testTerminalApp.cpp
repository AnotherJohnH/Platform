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


#include "TerminalApp.h"

#define  PROGRAM         "MyApp"
#define  DESCRIPTION     "A test"
#define  COPYRIGHT_YEAR  "2017"
#define  AUTHOR          "John D. Haughton"
#define  VERSION         PROJ_VERSION

class MyApp : public TerminalApp
{
private:
   // TODO remove this override if not required
   virtual void showExtraHelp() override
   {
      printf("Some extra help text.\n");
   }

   virtual int startTerminalApp(PLT::Device& term) override
   {
      // TODO Body of application goes here

      const char* text = "Hello, world!\n";
      term.write(text, strlen(text));

      char ch;
      while(term.read(&ch, 1) != -1);

      return 0;
   }

public:
   MyApp(int argc, const char* argv[])
      : TerminalApp(PROGRAM, AUTHOR, DESCRIPTION, VERSION, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   } 
};


int main(int argc, const char* argv[])
{
   MyApp(argc, argv);
}

