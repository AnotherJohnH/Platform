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

#ifndef ZLAUNCHER_H
#define ZLAUNCHER_H

#include <cstdio>
#include <cstring>

#include "PLT/Curses.h"
#include "PLT/KeyCode.h"

#include "ZifVersion.h"
#include "ZMachine.h"

//! ZMachine launcher
class ZLauncher : public PLT::Curses
{
private:
   PLT::Device&   term;
   const char*    config_file{nullptr};
   unsigned       cursor{0};
   unsigned       limit{0};
   bool           selection_is_dir{false};
   bool           quit{false};
   char           selection[FILENAME_MAX] = {};
   char           prefix[FILENAME_MAX] = {};

   bool getLine(FILE* fp, char* file, size_t size)
   {
      while(fgets(file, size, fp))
      {
         if (file[0] != '#')
         {
            char* s = strchr(file, '\n');
            if (s) *s = '\0';
            return true;
         }
      }

      return false;
   }

   void drawHeader()
   {
      clear();

      attron(PLT::A_REVERSE);
      move(1,1);
      clrtoeol();
      mvaddstr(1, 3, PROGRAM);
      attroff(PLT::A_REVERSE);

      attron(PLT::A_BOLD);
      mvaddstr(3, 3, prefix);
      attroff(PLT::A_BOLD);
   }

   void drawList()
   {
      char prev[FILENAME_MAX];
      prev[0] = '\0';

      FILE* fp = fopen(config_file, "r");
      if (fp == nullptr)
      {
         return;
      }

      for(unsigned line =0; line<lines; )
      {
         char text[FILENAME_MAX];

         if (!getLine(fp, text, sizeof(text)))
         {
            limit = line - 1;
            break;
         }

         if (strncmp(text, prefix, strlen(prefix)) == 0)
         {
            // Start of line matches current prefix
            char* suffix = text + strlen(prefix);

            bool is_dir = false;
            char* s = strchr(suffix, '/');
            if (s != nullptr)
            {
               *s = '\0';
               is_dir = true;
            }

            if (strcmp(suffix, prev) != 0)
            {
               strcpy(prev, suffix);

               if (line++ == cursor)
               {
                  strcpy(selection, suffix);
                  selection_is_dir = is_dir;

                  attron(PLT::A_REVERSE);
               }

               mvaddstr(4 + line, 3, suffix[0] == '!' ? suffix + 1 : suffix);

               attroff(PLT::A_REVERSE);
            }
         }
      }

      fclose(fp);
   }

   void openDir()
   {
      cursor = 0;
      strcat(prefix, selection);
      strcat(prefix, "/");
   }

   void closeDir()
   {
      char* s = strrchr(prefix, '/');
      if (s)
      {
         *s = '\0';

         s = strrchr(prefix, '/');
         if (s)
         {
            s[1] = '\0';
         }
         else
         {
            prefix[0] = '\0';
         }

         cursor = 0;
      }
   }

   void layoutText(unsigned l, unsigned c, const char* text)
   {
      move(l, c);

      const char* s = text;
      const char* o = s;

      for(unsigned x=3; true; x++)
      {
         if (isspace(*s) || (*s == '\0'))
         {
            if ((s - o) == 1)
            {
               addstr("\n\n");
               for(x = 1; x < c; ++x) addch(' ');
               o = s + 1;
            }
            else
            {
               if (x >= cols)
               {
                  addch('\n');
                  for(x = 1; x < c; ++x) addch(' ');
                  o++;
                  x += s - o;
               }

               for(; o != s; o++)
               {
                  char ch = *o;
                  if (ch == '\n') ch = ' ';
                  addch(ch);
               }
            }
         }

         if (*s++ == '\0') break;
      }
   }

   void doInfo()
   {
      drawHeader();

      mvaddstr( 3, 3, "Program     : "); addstr(PROGRAM);
      mvaddstr( 4, 3, "Description : "); addstr(DESCRIPTION);
      mvaddstr( 5, 3, "Author      : "); addstr(AUTHOR);
      mvaddstr( 6, 3, "Version     : "); addstr(VERSION);
      mvaddstr( 7, 3, "Built       : "); addstr(__TIME__); addstr(" "); addstr(__DATE__);
      mvaddstr( 8, 3, "Compiler    : "); layoutText(8, 17, __VERSION__);

      attron(PLT::A_BOLD);
      mvaddstr(10, 3, "Copyright (c) "); addstr(COPYRIGHT_YEAR); addstr(" "); addstr(AUTHOR);
      attroff(PLT::A_BOLD);

      layoutText(12, 3, LICENSE);

      (void) getch();
   }

   void action(const char* cmd)
   {
      if (cmd[0] == '!')
      {
         ++cmd;

         if (strcmp(cmd,  "QUIT") == 0)
         {
            quit = true;
         }
         else if (strcmp(cmd,  "INFO") == 0)
         {
            doInfo();
         }
         else if (strcmp(cmd,  "Inverse") == 0)
         {
            term.ioctl(PLT::Device::IOCTL_TERM_PALETTE, 0, 0xFFFFFF);
            term.ioctl(PLT::Device::IOCTL_TERM_PALETTE, 7, 0x000000);
         }
         else if (strcmp(cmd,  "Normal") == 0)
         {
            term.ioctl(PLT::Device::IOCTL_TERM_PALETTE, 0, 0x000000);
            term.ioctl(PLT::Device::IOCTL_TERM_PALETTE, 7, 0xFFFFFF);
         }
         else if (strncmp(cmd, "Border=", 7) == 0)
         {
            term.ioctl(PLT::Device::IOCTL_TERM_BORDER, atoi(cmd + 7));
         }
         else if (strncmp(cmd, "LineSpace=", 10) == 0)
         {
            term.ioctl(PLT::Device::IOCTL_TERM_LINE_SPACE, atoi(cmd + 10));
         }
         else if (strncmp(cmd, "FontSize=", 9) == 0)
         {
            term.ioctl(PLT::Device::IOCTL_TERM_FONT_SIZE, atoi(cmd + 9));

            Curses::init();
         }
      }
      else
      {
         char story[FILENAME_MAX];

         strcpy(story, prefix);
         strcat(story, cmd);

         run(story);
      }
   }

public:
   ZLauncher(PLT::Device& term_, const char* config_file_)
      : Curses(&term_)
      , term(term_)
      , config_file(config_file_)
   {
      action("!Border=0");
      action("!FontSize=18");
      action("!LineSpace=1");
      action("!Inverse");
   }

   int run(const char* story)
   {
      ZMachine(&term).open(story);
      return 0;
   }

   int menu()
   {
      while(!quit)
      {
         drawHeader();
         drawList();

         int key = getch();

         switch(key)
         {
         case PLT::ESCAPE:
            action("!QUIT");
            break;

         case PLT::UP:
            cursor = cursor == 0 ? limit : cursor - 1;
            break;

         case PLT::DOWN:
            cursor = cursor == limit ? 0 : cursor + 1;
            break;

         case '\n':
         case PLT::SELECT:
         case PLT::RIGHT:
            if (selection_is_dir)
            {
               openDir();
            }
            else
            {
               action(selection);
            }
            break;

         case PLT::LEFT:
            closeDir();
            break;

         default:
            quit = (key < 0);
            break;
         }
      }

      return 0;
   }
};

#endif
