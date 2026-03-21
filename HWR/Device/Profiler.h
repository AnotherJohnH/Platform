//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief 

#pragma once

#include <cstdio>

#if defined(HW_PROFILER)

#include "MTL/chip/Config.h"

extern uint8_t __text_start__;
extern uint8_t __text_end__;
extern uint8_t __data_start__;
extern uint8_t __bss_end__;

#else

// allow building completely non functional Usage
namespace MTL { using SysTick = uint32_t; }

const unsigned FLASH_SIZE = 100;
const unsigned RAM_SIZE   = 100;

#endif

namespace hw {

#if defined(HW_PROFILER)

template <bool ENABLE>
class Profiler
{
public:
   Profiler() = default;

   void start()
   {
      if (ENABLE)
      {
         t_start   = MTL_us_clock();
         non_usage = t_start - t_end;
      }
   }

   void stop()
   {
      if (ENABLE)
      {
         t_end = MTL_us_clock();
         usage = t_end - t_start;
      }
   }

   //! Get CPU usage (10x%)
   unsigned getCPUused() const
   {
      if (ENABLE)
      {
         return (usage * 1000) / (non_usage + usage);
      }

      return 0;
   }

   //! Get FLASH memory usage (%)
   unsigned getFLASHused() const
   {
      return unsigned(&__text_end__ - &__text_start__) * 100 / FLASH_SIZE;
   }

   //! Get RAM memory usage (%)
   unsigned getRAMused() const
   {
      return (&__bss_end__ - &__data_start__) * 100 / RAM_SIZE;
   }

   const char* format(char* text16_)
   {
      unsigned cpu_used = getCPUused();

#if 1
      snprintf(text16_, 16, "C%2u.%u%% F%2u%% R%2u%%",
              cpu_used / 10, cpu_used % 10,
              getFLASHused(),
              getRAMused());
#else
      snprintf(text16_, 16, "%8u %8u\n", usage >> 8, non_usage >> 8);
#endif

      return text16_;
   }

private:
   uint32_t          t_start{0};
   uint32_t          t_end{0};
   volatile uint32_t usage{0};
   volatile uint32_t non_usage{0};
};

#else

template <bool ENABLE>
class Profiler
{
public:
   Profiler() = default;

   void        start() {}
   void        stop() {}
   unsigned    getCPUused() { return 0; }
   unsigned    getFLASHused() { return 0; }
   unsigned    getRAMused() { return 0; }
   const char* format(char* text16_) { *text16_ = '\0'; return text16_; }
};

#endif

} // namespace hw
