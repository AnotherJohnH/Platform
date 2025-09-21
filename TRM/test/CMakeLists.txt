#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_executable(testTerminal testTerminal.cpp)
target_link_libraries(testTerminal GUI)

add_executable(testTerminalFrame testTerminalFrame.cpp)
target_link_libraries(testTerminalFrame GUI)

add_executable(testTerminalApp testTerminalApp.cpp)
target_link_libraries(testTerminalApp GUI)

add_executable(testTerminalLauncher testTerminalLauncher.cpp)
target_link_libraries(testTerminalLauncher GUI)

add_executable(testTerminalConsole testTerminalConsole.cpp)
target_link_libraries(testTerminalConsole PLT)
