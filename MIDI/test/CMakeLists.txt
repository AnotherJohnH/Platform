#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(testMidiFile testMidiFile.cpp)
   target_link_libraries(testMidiFile MIDI)

   add_executable(test_MIDI
                  testMain.cpp)

   target_link_libraries(test_MIDI MIDI)

   add_test(NAME test_MIDI COMMAND test_MIDI)

endif()
