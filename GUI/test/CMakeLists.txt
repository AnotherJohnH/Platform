#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_executable(testGuiFrame testGuiFrame.cpp)
target_link_libraries(testGuiFrame GUI)

add_executable(testGuiFrameBitmap testGuiFrameBitmap.cpp)
target_link_libraries(testGuiFrameBitmap GUI)

add_executable(testGui testGui.cpp)
target_link_libraries(testGui GUI)

add_executable(testGuiApp testGuiApp.cpp)
target_link_libraries(testGuiApp GUI)

add_executable(testGuiHelloWorld testGuiHelloWorld.cpp)
target_link_libraries(testGuiHelloWorld GUI)
