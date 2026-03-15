#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MIDI STATIC
            MidiDecoder.cpp)

target_include_directories(MIDI PUBLIC ..)

target_link_libraries(MIDI STB)

add_subdirectory(test)
