#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_executable(testAudioIn testAudioIn.cpp)
target_link_libraries(testAudioIn PLT)

add_executable(testAudioOut testAudioOut.cpp)
target_link_libraries(testAudioOut PLT)

add_executable(testEvent testEvent.cpp)
target_link_libraries(testEvent PLT)

add_executable(testFrame testFrame.cpp)
target_link_libraries(testFrame PLT)

add_executable(testFrameGenerator testFrameGenerator.cpp)
target_link_libraries(testFrameGenerator GUI)

add_executable(testMidiIn testMidiIn.cpp)
target_link_libraries(testMidiIn PLT STB)

add_executable(testSounder testSounder.cpp)
target_link_libraries(testSounder PLT)
