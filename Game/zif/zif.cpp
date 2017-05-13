//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#include  "STB/ConsoleApp.h"

#include  "PLT/TerminalPaper.h"
#include  "PLT/TerminalStdio.h"

#include  "ZifVersion.h"
#include  "ZLauncher.h"


class Zif : public STB::ConsoleApp
{
private:
   // Kindle3 resolution (pixels)
   static const unsigned TERM_WIDTH  = 600;
   static const unsigned TERM_HEIGHT = 800;

   STB::Option<const char*>  opt_config;
   STB::Option<bool>         opt_term;
   const char*               filename{nullptr};

   int launch(PLT::Device& term)
   {
      ZLauncher  launcher(term, opt_config);

      return filename ? launcher.run(filename)
                      : launcher.menu();
   }

   virtual int start() override
   {
      if (argc == 2)
      {
          filename = argv[1];
      }

      if (opt_term)
      {
         // Use the parent terminal
         PLT::TerminalStdio  term(PROGRAM);

         return launch(term);
      }
      else
      {
         // Use the built in terminal
         PLT::TerminalPaper<TERM_WIDTH,TERM_HEIGHT>  term(PROGRAM);

         return launch(term);
      }
   }

public:
   Zif(int argc_, const char* argv_[])
      : ConsoleApp(argc_, argv_,
                   PROGRAM, AUTHOR, DESCRIPTION, VERSION, COPYRIGHT_YEAR, LICENSE,
                   "[Z-file]")
      , opt_config( 'c', "config", "Config file", "zif.cfg")
      , opt_term(   't', "term",   "Use the parent terminal")
      , filename(nullptr)
   {
      parseArgsAndStart();
   }
};


int main(int argc, const char *argv[])
{
   Zif(argc, argv);
}

