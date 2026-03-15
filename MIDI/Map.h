//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>

namespace MIDI {

class Map
{
public:
    class ControlMap
    {
    public:
       ControlMap() = default;

       virtual const char* findControl(uint8_t  midi_control,
                                       unsigned controller_index) const = 0;
    };

   static void centerText(char* buffer, unsigned width, const char* text)
   {
      size_t n = strlen(text);

      while(text[n - 1] == ' ')
         --n;

      if (n > width)
      {
         memcpy(buffer, text, width);
      }
      else
      {
         for(unsigned i = 0; i < width; ++i)
            buffer[i] = ' ';
         memcpy(buffer + (width - n) / 2, text, n);
      }

      buffer[width] = '\0';
   }

   static void print(const char*    controller_name,
                     const uint8_t* midi_map,
                     ControlMap*    control_map,
                     unsigned       controller_index)
   {
      unsigned num_cols = midi_map[0];
      unsigned num_rows = midi_map[1];

      printf("%s map:\n\n", controller_name);

      for(unsigned row = 0; row < num_rows; ++row)
      {
         for(unsigned line = 0; line < 4; ++line)
         {
            printf("  ");

            for(unsigned col = 0; col < num_cols; ++col)
            {
               uint8_t midi = midi_map[2 + row * num_cols + col];

               if (midi == NONE)
               {
                  printf("        ");
               }
               else
               {
                  const char* label = control_map->findControl(midi, controller_index);
                  printKnob(line, midi, label);
               }
            }

            printf("\n");
        }

        printf("\n");
      }
   }

private:
   static void printKnob(unsigned line, unsigned midi, const char* label)
   {
      if (label == nullptr)
      {
         switch(line)
         {
         case 0: printf("        "); break;
         case 1: printf("  .--.  "); break;
         case 2: printf(" (    ) "); break;
         case 3: printf("  '--'  "); break;
         }
      }
      else
      {
         char text[8];
         centerText(text, 7, label);
   
         switch(line)
         {
         case 0: printf(" %s", text); break;
         case 1: printf("  .--.  "); break;
         case 2: printf(" (%3u ) ", midi); break;
         case 3: printf("  '--'  "); break;
         }
      }
   }
};

} // namespace MIDI
