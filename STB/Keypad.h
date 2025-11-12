//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Keypad interface with event FIFO

#pragma once

#include "STB/Fifo.h"

namespace STB {

template <unsigned NUM_KEYS,
          unsigned LOG2_FIFO_SIZE = 4,
          bool     IGNORE_UP = false>
class Keypad
{
   struct Event
   {
      uint8_t key;
      bool    down;
   };

public:
   //! Scan and pop next eevent
   bool popEvent(unsigned& key_, bool& down_)
   {
      if (manual_scan)
         keypadScan();

      if (fifo.empty())
         return false;

      key_  = fifo.back().key;
      down_ = fifo.back().down;
      fifo.pop();

      return true;
   }

   //! Scan and return current state
   bool operator[](unsigned key_)
   {
      if (manual_scan)
         keypadScan();

      return state[key_];
   }

protected:
   Keypad(bool manual_scan_)
      : manual_scan(manual_scan_)
   {
   }

   void pushEvent(unsigned key_, bool down_)
   {
      state[key_] = down_;

      if (down_ || not IGNORE_UP)
         fifo.push({uint8_t(key_), down_});
   }

   virtual bool keypadSample(unsigned key_) const { return false; }

   virtual void keypadScan()
   {
      for(unsigned key = 0; key < NUM_KEYS; ++key)
      {
         bool down = keypadSample(key);

         if (down != state[key])
         {
            pushEvent(key, down);
         }
      }
   }

private:
   bool                        manual_scan{};
   bool                        state[NUM_KEYS];
   Fifo<Event, LOG2_FIFO_SIZE> fifo{};
};

} // namespace STB
