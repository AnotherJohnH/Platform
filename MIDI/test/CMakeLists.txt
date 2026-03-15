#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(testMidiFile testMidiFile.cpp)
   target_link_libraries(testMidiFile MIDI)

endif()
