//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include "SDL.h"
#include "SDL_audio.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// The pixel format PLT is expecting to use
// XXX may not be the most effiicent?
#define PIXEL_FORMAT SDL_PIXELFORMAT_ARGB8888

