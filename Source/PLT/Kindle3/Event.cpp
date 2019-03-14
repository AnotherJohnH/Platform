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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

// C++11 threads would be nicer but don't seem to be able to link
// against any libs that actually work at runtime
#include <pthread.h>

#include "PLT/Event.h"
#include "STB/Fifo.h"


// ALT    ?

static const uint8_t DEL = PLT::BACKSPACE; // Del
static const uint8_t RET = PLT::RETURN;    // <-+
static const uint8_t LSH = PLT::LSHIFT;    // ^
static const uint8_t UP  = PLT::UP;        // Cursor up
static const uint8_t DWN = PLT::DOWN;      // Cursor down
static const uint8_t LFT = PLT::LEFT;      // Cursor left
static const uint8_t RGT = PLT::RIGHT;     // Cursor ritgh
static const uint8_t HME = PLT::ESCAPE;    // Home
static const uint8_t LPR = PLT::HOME;      // Left previous
static const uint8_t LNX = PLT::END;       // Left next
static const uint8_t RPR = PLT::PAGE_UP;   // Right previous
static const uint8_t RNX = PLT::PAGE_DOWN; // Right next
static const uint8_t SEL = PLT::SELECT;    // Select (center of cursor keys)
static const uint8_t MNU = PLT::MENU;      // Menu
static const uint8_t BAK = PLT::BACK;      // Back
static const uint8_t AA  = PLT::CAPSLOCK;  // Aa
static const uint8_t SYM = PLT::RALT;      // Sym
static const uint8_t VLD = PLT::VOL_DOWN;  // Volume -
static const uint8_t VLU = PLT::VOL_UP;    // Volume +


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


class EventImpl
{
private:
   static const uint16_t TIMER_EV = 0xFF00;

   pthread_t             key_td{0};
   pthread_mutex_t       fifo_mutex;
   pthread_cond_t        fifo_cv;
   STB::Fifo<uint16_t,4> fifo;

   pthread_t             timer_td{0};
   volatile uint32_t     timer_period_ms{0};
   pthread_mutex_t       timer_mutex;
   pthread_cond_t        timer_cv;

   bool                  sym_pressed{false};

   //! Push an event in a thread safe manner
   void pushEvent(PLT::Event::Type type, uint8_t code = 0)
   {
      uint16_t ev = (uint16_t(type) << 8) | code;

      pthread_mutex_lock(&fifo_mutex);

      if (!fifo.full()) fifo.push(ev);
      pthread_cond_signal(&fifo_cv);

      pthread_mutex_unlock(&fifo_mutex);
   }

   //! Get next event in a thread safe manner
   bool popEvent(bool block, PLT::Event::Type& type, uint8_t& code)
   {
      bool new_event;

      pthread_mutex_lock(&fifo_mutex);

      if (block)
      {
         while(fifo.empty())
         {
            pthread_cond_wait(&fifo_cv, &fifo_mutex);
         }

         new_event = true;
      }
      else
      {
         new_event = !fifo.empty();
      }

      if (new_event)
      {
         uint16_t ev = fifo.back();
         fifo.pop();
         type = PLT::Event::Type(ev >> 8);
         code = uint8_t(ev);
      }

      pthread_mutex_unlock(&fifo_mutex);

      return new_event;
   }

   void keyEvent(bool down, uint8_t code)
   {
      if (code == SYM)
      {
         sym_pressed = down;
      }
      else
      {
         if (sym_pressed)
         {
            switch(code)
            {
            case 'q': code = '1';  break;
            case 'w': code = '2';  break;
            case 'e': code = '3';  break;
            case 'r': code = '4';  break;
            case 't': code = '5';  break;
            case 'y': code = '6';  break;
            case 'u': code = '7';  break;
            case 'i': code = '8';  break;
            case 'o': code = '9';  break;
            case 'p': code = '0';  break;

            case 'a': code = '-';  break;
            case 's': code = '=';  break;
            case 'd': code = '[';  break;
            case 'f': code = ']';  break;
            case 'j': code = ';';  break;
            case 'k': code = '\''; break;
            case 'l': code = '\\'; break;

            case 'z': code = '`';  break;
            case 'n': code = ',';  break;
            case 'm': code = '/';  break;
            }
         }

         pushEvent(down ? PLT::Event::KEY_DOWN : PLT::Event::KEY_UP, code);
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
      for(unsigned i = 0; i < NUM_FD; ++i)
      {
         if(fd[i] > nfds) nfds = fd[i];
      }
      nfds++;

      while(true)
      {
         fd_set read_fds;

         FD_ZERO(&read_fds);

         for(unsigned i = 0; i < NUM_FD; ++i)
         {
            FD_SET(fd[i], &read_fds);
         }

         if(select(nfds, &read_fds, NULL, NULL, NULL) < 0) break;

         for(unsigned i = 0; i < NUM_FD; ++i)
         {
            if(FD_ISSET(fd[i], &read_fds))
            {
               uint8_t buffer[16];

               if(read(fd[i], buffer, 16) != 16) break;

               if(buffer[10] < 0xD0)
               {
                  switch(buffer[12])
                  {
                  case 0: keyEvent(/* down */ false, event_decode[buffer[10]]); break;
                  case 1: keyEvent(/* down */ true,  event_decode[buffer[10]]); break;
                  default: break;
                  }
               }
            }
         }
      }

      for(unsigned i = 0; i < NUM_FD; ++i)
      {
         close(fd[i]);
      }
   }

   static void* thunkKeyEventLoop(void* ptr)
   {
      EventImpl* impl = (EventImpl*)ptr;
      while(true)
      {
         impl->keyEventLoop();
      }
      return nullptr;
   }

   //! Loop to wait for periodic timer events
   void timerEventLoop()
   {
      pthread_mutex_lock(&timer_mutex);

      while(true)
      {
         while(timer_period_ms == 0)
         {
            // Wait for timer to be started
            pthread_cond_wait(&timer_cv, &timer_mutex);
         }

         while(timer_period_ms != 0)
         {
            // Get current time
            struct timeval now;
            gettimeofday(&now, nullptr);

            // Add timer period to the current time
            struct timespec then;
            unsigned delay_ms = timer_period_ms % 1000;
            then.tv_nsec = (now.tv_usec + delay_ms*1000) * 1000;
            unsigned delay_s  = timer_period_ms / 1000;
            then.tv_sec  = now.tv_sec + delay_s;
            if (then.tv_nsec >= 1000000000)
            {
               then.tv_nsec -= 1000000000;
               then.tv_sec  += 1;
            }

            // Wait for timer to expire or be reset
            int status = pthread_cond_timedwait(&timer_cv, &timer_mutex, &then);
            if (status == ETIMEDOUT)
            {
               // Post a timer event
               pushEvent(PLT::Event::TIMER);
            }
         }
      }
   }

   static void* thunkTimerEventLoop(void* ptr)
   {
      EventImpl* impl = (EventImpl*)ptr;
      impl->timerEventLoop();
      return nullptr;
   }

public:
   EventImpl()
   {
      pthread_mutex_init(&fifo_mutex, 0);

      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_create(&key_td, &attr, thunkKeyEventLoop, this);
   }

   ~EventImpl()
   {
      pthread_cancel(key_td);
      pthread_cancel(timer_td);
   }

   void setTimer(uint32_t period_ms)
   {
      if(period_ms == timer_period_ms) return;

      if(timer_td == 0)
      {
         pthread_attr_t attr;
         pthread_attr_init(&attr);
         pthread_create(&timer_td, &attr, thunkTimerEventLoop, this);
      }

      pthread_mutex_lock(&timer_mutex);

      timer_period_ms = period_ms;
      pthread_cond_signal(&timer_cv);

      pthread_mutex_unlock(&timer_mutex);
   }

   PLT::Event::Type getEvent(PLT::Event::Message& event, bool block)
   {
      event.type = PLT::Event::NONE;
      event.x    = 0;
      event.y    = 0;

      if(popEvent(block, event.type, event.code))
      {
         // printf("getEvent() => EV %x:%02x\n", event.type, event.code);
      }

      return PLT::Event::Type(event.type);
   }
};


static EventImpl impl;


static PLT::Event::Type getEvent(PLT::Event::Message& event, bool wait)
{
   return impl.getEvent(event, wait);
}


namespace PLT {

namespace Event {

Type poll(Message& event)
{
   return getEvent(event, /* block */ false);
}

Type wait(Message& event)
{
   return getEvent(event, /* block */ true);
}

int mainLoop(bool (*callback)(void*), void* user_data)
{
   while(true)
   {
      Message event;
      Type    type = wait(event);

      if(callback != nullptr) (*callback)(user_data);

      if(type == QUIT) return 0;
   }
}

int eventLoop(void (*callback)(const Message&, void*), void* user_data)
{
   while(true)
   {
      Message event;
      Type    type = wait(event);

      if(callback != nullptr) (*callback)(event, user_data);

      if(type == QUIT) return 0;
   }
}

void setTimer(unsigned period_ms)
{
   impl.setTimer(period_ms);
}


} // namespace Event

} // namespace PLT
