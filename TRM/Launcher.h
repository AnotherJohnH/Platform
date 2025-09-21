//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// XXX time to drop the zero dynamic allocation restriction from this code

#pragma once

#include <cctype>
#include <cstring>

#include <string>
#include <utility>

#include "PLT/File.h"
#include "PLT/KeyCode.h"

#include "STB/Oil.h"

#include "TRM/App.h"
#include "TRM/Curses.h"

namespace TRM {

struct TermConfig : public STB::Oil<TermConfig>
{
   unsigned font_size{18};
   unsigned border_pixels{0};
   unsigned line_space{0};
   bool     invert_video{false};
   unsigned sleep{1};
#ifdef PLT_TARGET_Kindle3
   uint32_t bg_colour{0xFFFFFF};
   uint32_t fg_colour{0x000000};
#else
   uint32_t bg_colour{0xF0F0E0};
   uint32_t fg_colour{0x382800};
#endif
};

BOIL(TermConfig)
{
   MOIL(font_size);
   MOIL(border_pixels);
   MOIL(line_space);
   MOIL(sleep);
   MOIL(bg_colour); FOIL(bg_colour, HEX);
   MOIL(fg_colour); FOIL(fg_colour, HEX);
}
EOIL(TermConfig)


class Launcher : public App
{
public:
   Launcher(const char* program,
            const char* description,
            const char* link,
            const char* author,
            const char* copyright_year,
            const char* args_help,
            const char* config_file)
      : App(program, description, link, author, copyright_year)
      , opt_config('c', "config", "Use alternate config file", config_file)
      , filename('*', "*", args_help, nullptr)
   {
   }

protected:
   Device*     term{nullptr};
   TRM::Curses curses;

private:
   STB::Option<const char*> opt_config;
   STB::Option<const char*> filename;
   TermConfig               config;
   unsigned                 cursor{0};
   unsigned                 cursor_limit{0};
   bool                     quit{false};
   std::string              path {};
   char                     selection[FILENAME_MAX] = {};
   bool                     selection_is_dir{false};

   //! Get the next line with content from the given file stream
   static bool getLine(PLT::File& file, char* buffer, size_t size)
   {
      while(file.getLine(buffer, size))
      {
         if(buffer[0] != '#')
         {
            char* s = strchr(buffer, '\n');
            if(s) *s = '\0';
            return true;
         }
      }

      return false;
   }

   //!
   void drawHeader()
   {
      curses.clear();

      curses.attron(A_REVERSE);

      curses.move(1, 1);
      for(unsigned i = 0; i < curses.cols; ++i)
      {
         curses.addch(' ');
      }

      curses.attron(A_BOLD);
      curses.mvaddstr(1, 3, program);
      curses.attroff(A_BOLD);

      curses.mvaddstr(1, 3 + strlen(program) + 2, path.c_str());

      curses.attroff(A_REVERSE);
   }

   //!
   void drawList()
   {
      selection_is_dir = false;
      strcpy(selection, "!Quit");

      const unsigned first_row = 3;

      PLT::File file(nullptr, opt_config);

      if(!file.openForRead())
      {
         curses.mvaddstr(first_row, 3, "ERROR - failed to open \"");
         curses.addstr(file.getFilename());
         curses.addstr("\"");
         return;
      }

      char prev[FILENAME_MAX];
      prev[0] = '\0';

      for(unsigned index = 0; (first_row + index) < curses.lines;)
      {
         // Read one line from the config file
         char line[FILENAME_MAX];
         if(!getLine(file, line, sizeof(line)))
         {
            cursor_limit = index - 1;
            break;
         }

         // Does the start of the line match the current path
         if(strncmp(line, path.c_str(), path.size()) == 0)
         {
            char* entry = line + path.size();

            // Extract entry and check if it is a directory
            bool  is_dir = false;
            char* slash  = strchr(entry, '/');
            if(slash != nullptr)
            {
               *slash = '\0';
               is_dir = true;
            }

            // Is this entry different to the previous entry
            if(strcmp(entry, prev) != 0)
            {
               strcpy(prev, entry);

               // Is this the currently selected entry
               if(index++ == cursor)
               {
                  strcpy(selection, entry);
                  selection_is_dir = is_dir;

                  curses.attron(A_REVERSE);
               }

               if(entry[0] == '!')
               {
                  entry++;
               }

               curses.mvaddstr(first_row + index, 3, entry);

               curses.attroff(A_REVERSE);
            }
         }
      }
   }

   void layoutText(unsigned l, unsigned c, const char* text)
   {
      curses.move(l, c);

      const char* s = text;
      const char* o = s;

      for(unsigned x = 3; true; x++)
      {
         if(isspace(*s) || (*s == '\0'))
         {
            if((s - o) == 1)
            {
               curses.addstr("\n\n");
               for(x = 1; x < c; ++x)
                  curses.addch(' ');
               o = s + 1;
            }
            else
            {
               if(x >= curses.cols)
               {
                  curses.addch('\n');
                  for(x = 1; x < c; ++x)
                     curses.addch(' ');
                  o++;
                  x += s - o;
               }

               for(; o != s; o++)
               {
                  char ch           = *o;
                  if(ch == '\n') ch = ' ';
                  curses.addch(ch);
               }
            }
         }

         if(*s++ == '\0') break;
      }
   }

   //! Display info page
   void doInfo()
   {
      drawHeader();

      curses.mvaddstr(3, 3, "Program     : "); curses.addstr(program);
      curses.mvaddstr(4, 3, "Description : "); curses.addstr(description);
      curses.mvaddstr(5, 3, "Link        : "); if (link != nullptr) curses.addstr(link);
      curses.mvaddstr(6, 3, "Author      : "); curses.addstr(author);
      curses.mvaddstr(7, 3, "Version     : "); curses.addstr(PLT_VERSION);

      curses.mvaddstr(8, 3, "Built       : ");
      curses.addstr(__TIME__); curses.addstr(" "); curses.addstr(__DATE__);

      curses.mvaddstr(9, 3, "Compiler    : "); layoutText(9, 17, __VERSION__);

      curses.attron(A_BOLD);
      curses.mvaddstr(11, 3, "Copyright (c) ");
      curses.addstr(copyright_year);
      curses.addstr(" ");
      curses.addstr(author);

      curses.attroff(A_BOLD);

      layoutText(13, 3, MIT_LICENSE);

      (void)curses.getch();
   }

   //! Implement an action
   void doAction(const char* cmd, const char* value = "")
   {
      if(strcmp(cmd, "Quit") == 0)
      {
         quit = true;
      }
      else if(strcmp(cmd, "Info") == 0)
      {
         doInfo();
      }
      else
      {
         if(strcmp(cmd, "Video") == 0)
         {
            bool invert = strcmp(value, "Inverse") == 0;

            if(config.invert_video != invert)
            {
               config.invert_video = invert;
               std::swap(config.bg_colour, config.fg_colour);
            }
         }
         else if(strcmp(cmd, "Colours") == 0)
         {
            if(strcmp(value, "GreenPhosphor") == 0)
            {
               config.bg_colour = 0x000000;
               config.fg_colour = 0x40FF40;
            }
            else if(strcmp(value, "AmberPhosphor") == 0)
            {
               config.bg_colour = 0x000000;
               config.fg_colour = 0xFFC000;
            }
            else if(strcmp(value, "BluePhosphor") == 0)
            {
               config.bg_colour = 0x000000;
               config.fg_colour = 0xD0F0FF;
            }
            else if(strcmp(value, "OldPaper") == 0)
            {
               config.bg_colour = 0xF0F0E0;
               config.fg_colour = 0x382800;
            }
            else if(strcmp(value, "White") == 0)
            {
               config.bg_colour = 0x000000;
               config.fg_colour = 0xFFFFFF;
            }
         }
         else if(strcmp(cmd, "Border") == 0)
         {
            config.border_pixels = atoi(value);
         }
         else if(strcmp(cmd, "LineSpace") == 0)
         {
            config.line_space = atoi(value);
         }
         else if(strcmp(cmd, "FontSize") == 0)
         {
            config.font_size = atoi(value);
         }
         else if(strcmp(cmd, "Sleep") == 0)
         {
            config.sleep = atoi(value) * 60;
         }

         configTerminal();

         config.write();
      }
   }

   //! update the terminal configuration
   void configTerminal()
   {
      term->ioctl(Device::IOCTL_TERM_PALETTE, 0, config.bg_colour);
      term->ioctl(Device::IOCTL_TERM_PALETTE, 1, config.fg_colour);
      term->ioctl(Device::IOCTL_TERM_BORDER, config.border_pixels);
      term->ioctl(Device::IOCTL_TERM_LINE_SPACE, config.line_space);
      term->ioctl(Device::IOCTL_TERM_FONT_SIZE, config.font_size);
      term->ioctl(Device::IOCTL_TERM_SLEEP, config.sleep);

      curses.init();
   }

   //! Open a config directory (not a real directory)
   void openDir(const char* sub_directory)
   {
      path += sub_directory;
      path += '/';
      cursor = 0;
   }

   //! Close a config directory (not a real directory)
   void closeDir()
   {
      size_t pos = path.rfind('/');
      if (pos != std::string::npos)
      {
         path.resize(pos);

         pos = path.rfind('/');
         if (pos != std::string::npos)
         {
            path.resize(pos + 1);
         }
         else
         {
            path = "";
         }

         cursor = 0;
      }
   }

   //! Select
   void doSelect(char* selection)
   {
      if(selection[0] == '!')
      {
         char* cmd = selection + 1;

         char* value = strchr(cmd, '=');
         if(value)
         {
            *value = '\0';

            doAction(cmd, value + 1);
         }
         else
         {
            doAction(cmd);
         }
      }
      else
      {
         // Launch selection

         char story[FILENAME_MAX];

         strcpy(story, path.c_str());
         strcat(story, selection);

         term->ioctl(Device::IOCTL_TERM_CURSOR, 1);

         startTerminalLauncher(story);

         term->ioctl(Device::IOCTL_TERM_CURSOR, 0);
      }
   }

   virtual int startTerminalApp(Device& term_) override
   {
      term = &term_;
      curses.setDevice(&term_);

      if (config.exists())
      {
         config.read();
      }

      configTerminal();

      return filename ? startTerminalLauncher(filename)
                      : menu();
   }

   //! Load and run a file
   virtual int startTerminalLauncher(const char* file) = 0;

   //! Enter the menu system
   int menu()
   {
      term->ioctl(Device::IOCTL_TERM_CURSOR, 0);

      while(!quit)
      {
         drawHeader();
         drawList();

         int key = curses.getch();

         switch(key)
         {
         case PLT::ESCAPE:
            doAction("Quit");
            break;

         case PLT::UP:
            cursor = cursor == 0 ? cursor_limit : cursor - 1;
            break;

         case PLT::DOWN:
            cursor = cursor == cursor_limit ? 0 : cursor + 1;
            break;

         case '\n':
         case PLT::SELECT:
         case PLT::RIGHT:
         case PLT::PAGE_DOWN:
            if(selection_is_dir)
            {
               openDir(selection);
            }
            else
            {
               doSelect(selection);
            }
            break;

         case PLT::PAGE_UP:
         case PLT::LEFT:
            closeDir();
            break;

         default:
            if(key < 0)
            {
               doAction("Quit");
            }
            break;
         }
      }

      curses.clear();

      return 0;
   }
};

} // namespace TRM
