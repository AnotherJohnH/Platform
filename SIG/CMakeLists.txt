#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

set(SIG_SAMPLE_RATE    "48000" CACHE STRING "SIG library sample rate (Hz)")
set(SIG_GAIN_LOG2_SIZE "15"    CACHE STRING "SIG library log2(gain_table_size)")

message("SIG_SAMPLE_RATE    = ${SIG_SAMPLE_RATE} Hz")
message("SIG_GAIN_LOG2_SIZE = ${SIG_GAIN_LOG2_SIZE}")
message("--------------------------------------------------------------------------------")

autogen_py(Table_note ${SIG_SAMPLE_RATE})
autogen_py(Table_gain ${SIG_GAIN_LOG2_SIZE})
autogen_py(Table_clip_tanh)

set(SIG_VARIANTS
    FL32
    FL64)

# FX32 is not working yet

foreach(variant IN LISTS SIG_VARIANTS)

   set(lib SIG_${variant})

   add_library(${lib} STATIC
               Table_note.cpp
               Table_gain.cpp
               Table_clip_tanh.cpp)

   target_compile_definitions(${lib} PUBLIC ${lib})

   target_include_directories(${lib} PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR})

   target_compile_definitions(${lib} PUBLIC SIG_SAMPLE_RATE=${SIG_SAMPLE_RATE})

endforeach()

add_subdirectory(test)
