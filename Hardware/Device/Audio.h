//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Audio out interface with getSamples() call-back

#pragma once

#if defined(HW_DAC_I2S_GENERIC) || defined(HW_DAC_I2S_WAVESHARE_REV2_1)
#define HW_DAC_I2S
#include "MTL/chip/PioAudio.h"

#elif defined(HW_DAC_PWM)
#include "MTL/chip/PwmAudio.h"

#elif defined(HW_DAC_NATIVE)
#include "PLT/Audio.h"

#else
#include <cstdint>

#endif

namespace hw {

#if defined(HW_DAC_I2S_GENERIC)

//! Generic I2S DAC
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq, bool stereo_pairs_ = false)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  HW_DAC_I2S_SD,
                                                  HW_DAC_I2S_CLKS,
                                                  /* MCLK */ MTL::PIN_IGNORE,
                                                  stereo_pairs_ ? MTL::Audio::STEREO_PAIRS_16
                                                                : MTL::Audio::STEREO_16,
                                                  /* LSB LRCLK / MSB SCLK */ false}
   {
      MTL::config.gpio(HW_DAC_I2S_SD,       ">I2S SD");
      MTL::config.gpio(HW_DAC_I2S_CLKS,     ">I2S SCLK");
      MTL::config.gpio(HW_DAC_I2S_CLKS + 1, ">I2S LRCLK");
   }
};

#elif defined(HW_DAC_I2S_WAVESHARE_REV2_1)

//! Waveshare Pico-Audio (Rev 2.1) I2S DAC
template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq, bool stereo_pairs_ = false)
      : MTL::PioAudio<MTL::Pio0,SAMPLES_PER_TICK>{dac_freq,
                                                  HW_DAC_I2S_SD,
                                                  HW_DAC_I2S_CLKS,
                                                  HW_DAC_I2S_MCLK,
                                                  stereo_pairs_ ? MTL::Audio::STEREO_PAIRS_16
                                                                : MTL::Audio::STEREO_16,
                                                  /* LSB LRCLK / MSB SCLK */ true}
   {
      MTL::config.gpio(HW_DAC_I2S_SD,       ">I2S SD");
      MTL::config.gpio(HW_DAC_I2S_CLKS,     ">I2S LRCLK");
      MTL::config.gpio(HW_DAC_I2S_CLKS + 1, ">I2S SCLK");
      MTL::config.gpio(HW_DAC_I2S_MCLK,     ">I2S MCLK");
   }
};

#elif defined(HW_DAC_PWM)

template <unsigned SAMPLES_PER_TICK>
class Audio : public MTL::PwmAudio<HW_DAC_PWM, /* BITS */ 8, SAMPLES_PER_TICK>
{
public:
   Audio(unsigned dac_freq, bool stereo_pairs_ = false)
      : MTL::PwmAudio<HW_DAC_PWM, /* BITS */ 8, SAMPLES_PER_TICK>{dac_freq}
   {
      MTL::config.gpio(HW_DAC_PWM, ">PWM (audio)");
   }
};

#elif defined(HW_DAC_NATIVE)

template <unsigned SAMPLES_PER_TICK>
class Audio : public PLT::Audio::Out
{
public:
   // XXX requested DAC frequency ignored
   Audio(unsigned dac_freq, bool stereo_pairs_ = false)
      : PLT::Audio::Out(dac_freq,
                        PLT::Audio::Format::SINT16,
                        /* channels */ 2,
                        /* samples */ SAMPLES_PER_TICK)
   {}

   void irqHandler() {}

private:
   void getSamples32(uint32_t* buffer, unsigned n);

   void getSamples(int16_t* buffer, unsigned n) override
   {
      getSamples32((uint32_t*)buffer, n / 2);
   }
};

#else

template <unsigned SAMPLES_PER_TICK>
class Audio
{
public:
   Audio(unsigned dac_freq_, bool stereo_piars_) {}

   void start() {}

protected:
   void getSamples(int16_t* buffer, unsigned n);
};

#endif

} // namespace hw
