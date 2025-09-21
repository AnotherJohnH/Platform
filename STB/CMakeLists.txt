#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(STB STATIC
            Deflate.cpp
            MidiDecoder.cpp
            Oil.cpp
            Option.cpp
            Zlib.cpp)

target_include_directories(STB PUBLIC ..)

add_subdirectory(test)
