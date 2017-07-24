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

#ifndef TERMINAL_APP_H
#define TERMINAL_APP_H

#include  "STB/ConsoleApp.h"

#include  "PLT/TerminalPaper.h"
#include  "PLT/TerminalStdio.h"


class TerminalApp : public STB::ConsoleApp
{
private:
#ifndef NO_TERMINAL_EMULATOR
   enum Display
   {  
      DISP_KINDLE3,
      DISP_VGA,
      DISP_SVGA,
      DISP_XGA
   };

#ifdef PROJ_TARGET_Kindle3
   Display display = DISP_KINDLE3;
#else
   Display display = DISP_SVGA;
#endif

   STB::Option<bool>         opt_term{'t', "term", "Use the parent terminal (not the built in terminal)"};
   STB::Option<bool>         opt_k3{  'K', "k3",   "Kindle 3 display 600x800"};
   STB::Option<bool>         opt_vga{ 'V', "vga",  "VGA display      640x480"};
   STB::Option<bool>         opt_svga{'S', "svga", "SVGA display     800x600"};
   STB::Option<bool>         opt_xga{ 'X', "xga",  "XGA display     1024x768"};
#endif

   virtual int startConsoleApp() override
   {  
#ifndef NO_TERMINAL_EMULATOR
      if (opt_term)
#endif
      {  
         // Use the parent terminal
         PLT::TerminalStdio  term(program);
         return startTerminalApp(term);
      }
#ifndef NO_TERMINAL_EMULATOR
      else
      {  
         // Use the built in terminal
              
              if (opt_k3)   display = DISP_KINDLE3;
         else if (opt_vga)  display = DISP_VGA;
         else if (opt_svga) display = DISP_SVGA;
         else if (opt_xga)  display = DISP_XGA;
         
         switch(display)
         {
         case DISP_KINDLE3: return launchDisplay< 600,800>();
         case DISP_VGA:     return launchDisplay< 640,480>();
         case DISP_SVGA:    return launchDisplay< 800,600>();
         case DISP_XGA:     return launchDisplay<1024,768>();
         }
         
         assert(!"Display selection bug");
         return 1;
      }
#endif
   }

   template <unsigned WIDTH, unsigned HEIGHT>
   int launchDisplay()
   {  
      PLT::TerminalPaper<WIDTH,HEIGHT>  term(program);
      return startTerminalApp(term);
   }

protected:
   //! App entry point with constructed terminal
   virtual int startTerminalApp(PLT::Device& term) = 0;

public:
    TerminalApp(const char*  program_,
                const char*  author_,
                const char*  description_,
                const char*  version_,
                const char*  copyright_year_,
                const char*  args_help_ = nullptr)
      : ConsoleApp(program_, author_, description_, version_, copyright_year_, args_help_)
   {}
};

#endif
