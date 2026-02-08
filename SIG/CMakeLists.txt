#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

set(SIG_SAMPLE_RATE    "48000" CACHE STRING "SIG library sample rate (Hz)")
set(SIG_GAIN_LOG2_SIZE "15"    CACHE STRING "SIG library log2(gain_table_size)")

message("SIG_SAMPLE_RATE    = ${SIG_SAMPLE_RATE} Hz")
message("SIG_GAIN_LOG2_SIZE = ${SIG_GAIN_LOG2_SIZE}")

autogen_py(Table_note ${SIG_SAMPLE_RATE})
autogen_py(Table_gain ${SIG_GAIN_LOG2_SIZE})
autogen_py(Table_clip_tanh)

add_library(SIG STATIC
            Table_note.cpp
            Table_gain.cpp
            Table_clip_tanh.cpp)

target_include_directories(SIG PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(SIG PUBLIC SIG_SAMPLE_RATE=${SIG_SAMPLE_RATE})

add_subdirectory(test)
