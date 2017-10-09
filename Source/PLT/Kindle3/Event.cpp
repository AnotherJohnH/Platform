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

// Kindle3 Linux event

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <assert.h>

#include "PLT/Event.h"
#include "STB/Fifo.h"


// ALT    ?

static const uint8_t DEL = PLT::BACKSPACE;
static const uint8_t RET = PLT::RETURN;
static const uint8_t LSH = PLT::LSHIFT;
static const uint8_t UP  = PLT::UP;            // Cursor up
static const uint8_t DWN = PLT::DOWN;          // Cursor down
static const uint8_t LFT = PLT::LEFT;          // Cursor left
static const uint8_t RGT = PLT::RIGHT;         // Cursor ritgh
static const uint8_t HME = PLT::ESCAPE;        // Home
static const uint8_t LPR = PLT::HOME;          // Left previous
static const uint8_t LNX = PLT::END;           // Left next
static const uint8_t RPR = PLT::PAGE_UP;       // Right previous
static const uint8_t RNX = PLT::PAGE_DOWN;     // Right next
static const uint8_t SEL = PLT::SELECT;        // Select (center of cursor keys)
static const uint8_t MNU = PLT::MENU;          // Menu
static const uint8_t BAK = PLT::BACK;          // Back
static const uint8_t AA  = PLT::CAPSLOCK;      // Aa
static const uint8_t SYM = PLT::RALT;          // Sym
static const uint8_t VLD = PLT::VOL_DOWN;      // Volume -
static const uint8_t VLU = PLT::VOL_UP;        // Volume +


static const uint8_t event_decode[0xD0] =
{
//  x0   x1   x2   x3   x4   x5   x6   x7   x8   x9   xA   xB   xC   xD   xE   xF
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, DEL,   0,  // 0x
   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   0,   0, RET,   0, 'a', 's',  // 1x
   'd', 'f', 'g', 'h', 'j', 'k', 'l',   0,   0,   0, LSH,   0, 'z', 'x', 'c', 'v',  // 2x
   'b', 'n', 'm',   0, '.',   0,   0,   0,   0, ' ',   0,   0,   0,   0,   0,   0,  // 3x
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 4x
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 5x
     0,   0,   0,   0,   0,   0, HME,  UP, LNX, LFT, RGT,   0, DWN, RPR,   0,   0,  // 6x
     0,   0, VLD, VLU,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, SYM,   0,  // 7x
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, MNU,   0,   0,   0,   0,  // 8x
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, BAK,   0,  // 9x
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // Ax
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  AA, RNX,  // Bx
     0, LPR, SEL,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // Cx
};


namespace PLT {

class EventImpl
{
private:
   static const uint16_t TIMER_EV = 0xFF00;

   pthread_t              key_td;
   pthread_t              tmr_td;
   volatile uint32_t      tmr_period_ms;
   pthread_mutex_t        fifo_mutex;
   STB::Fifo<uint16_t,4>  fifo;

   //! Push an event in a thread safe manner
   void pushEvent(EventType type, uint8_t code = 0)
   {
      uint16_t ev  = (uint16_t(type) << 8) | code;

      pthread_mutex_lock(&fifo_mutex);
      fifo.push(ev);
      pthread_mutex_unlock(&fifo_mutex);
   }

   //! Get next event in a thread safe manner
   bool popEvent(bool block, PLT::EventType& type, uint8_t& code)
   {
      while(true)
      {
         uint16_t ev;

         pthread_mutex_lock(&fifo_mutex);
         bool ok = !fifo.empty();
         if (ok)
         {
            ev = fifo.back();
            fifo.pop();
         }
         pthread_mutex_unlock(&fifo_mutex);

         if (ok)
         {
            type = PLT::EventType(ev >> 8);
            code = uint8_t(ev);
            return true;
         }

         if (!block)
         {
            return false;
         }
      }
   }

   //! Loop to wait for key events
   void keyEventLoop()
   {
      static const unsigned NUM_FD = 3;

      int fd[NUM_FD];

      fd[0] = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
      fd[1] = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
      fd[2] = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);

      int nfds = 0;
      for(unsigned i=0; i<NUM_FD; ++i)
      {
         if (fd[i] > nfds) nfds = fd[i];
      }
      nfds++;

      while(true)
      {
         fd_set  read_fds;

         FD_ZERO(&read_fds);

         for(unsigned i=0; i<NUM_FD; ++i)
         {
            FD_SET(fd[i], &read_fds);
         }

         if (select(nfds, &read_fds, NULL, NULL, NULL) < 0) break;

         for(unsigned i=0; i<NUM_FD; ++i)
         {
            if (FD_ISSET(fd[i], &read_fds))
            {
               uint8_t  buffer[16];

               if (read(fd[i], buffer, 16) != 16) break;;

               if (buffer[10] < 0xD0)
               {
                  switch(buffer[12])
                  {
                  case 0: pushEvent(KEY_UP,   event_decode[buffer[10]]); break;
                  case 1: pushEvent(KEY_DOWN, event_decode[buffer[10]]); break;
                  default: break;
                  }
               }
            }
         }
      }

      for(unsigned i=0; i<NUM_FD; ++i)
      {
         close(fd[i]);
      }
   }

   static void* thunkKeyEventLoop(void* ptr)
   {
       EventImpl* impl = (EventImpl*)ptr;
       impl->keyEventLoop();
       return 0;
   }

   //! Loop to wait for periodic timer events
   void tmrEventLoop()
   {
      while(true)
      {
         usleep(tmr_period_ms * 1000);
         pushEvent(TIMER);
      }
   }

   static void* thunkTmrEventLoop(void* ptr)
   {
       EventImpl* impl = (EventImpl*)ptr;
       impl->tmrEventLoop();
       return 0;
   }

public:
   EventImpl()
      : key_td(0)
      , tmr_td(0)
      , tmr_period_ms(0)
   {
      pthread_mutex_init(&fifo_mutex, 0);

      pthread_attr_t  attr;
      pthread_attr_init(&attr);
      pthread_create(&key_td, &attr, thunkKeyEventLoop, this);
   }

   ~EventImpl()
   {
      pthread_cancel(key_td);
      pthread_cancel(tmr_td);
   }

   void setTimer(uint32_t period_ms)
   {
      tmr_period_ms = period_ms;

      if (tmr_td == 0)
      {
         pthread_attr_t  attr;
         pthread_attr_init(&attr);
         pthread_create(&tmr_td, &attr, thunkTmrEventLoop, this);
      }
   }

   EventType getEvent(Event& event, bool block)
   {
      event.type = NONE;
      event.x    = 0;
      event.y    = 0;

      if (popEvent(block, event.type, event.code))
      {
         //printf("getEvent() => EV %x:%02x\n", event.type, event.code);
      }

      return EventType(event.type);
   }
};


static EventImpl  impl;


static EventType getEvent(Event& event, bool wait)
{
   return impl.getEvent(event, wait);
}

EventType pollEvent(Event& event)
{
   return getEvent(event, /* block */ false);
}

EventType waitEvent(Event& event)
{
   return getEvent(event, /* block */ true);
}

int eventLoop(void (*callback)(const Event&, void*), void* user_data)
{
   while(true)
   {
      Event     event;
      EventType type = waitEvent(event);

      if (callback != nullptr) (*callback)(event, user_data);

      if (type == QUIT) return 0;
   }
}

void setTimer(unsigned period_ms)
{
   impl.setTimer(period_ms);
}

} // namespace PLT
