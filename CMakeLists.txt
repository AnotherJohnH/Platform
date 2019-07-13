#-------------------------------------------------------------------------------
#  Copyright (c) 2019 John D. Haughton
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

set(app     PlatformTest)
set(version 0.0.4)

#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)
include(build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------
# GoogleTest
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)
link_directories(${CMAKE_SOURCE_DIR}/googletest/build/lib)

#-------------------------------------------------------------------------------

add_executable(testAudioOut Source/PLT/test/testAudioOut.cpp)
target_link_libraries(testAudioOut ${platform_libs})

add_executable(testGui Source/GUI/testGui.cpp)
target_link_libraries(testGui ${platform_libs})

add_executable(testPLT
               Source/STB/testEndian.cpp
               Source/test/testMain.cpp)
target_link_libraries(testPLT gtest ${platform_libs})

# TODO can we just iterate over a list of source
#              'PLT/test/testAudioIn.cpp',
#              'PLT/test/testMidiIn.cpp',
#              'PLT/test/testSounder.cpp',
#              'PLT/test/testEvent.cpp',
#              'PLT/test/testFrame.cpp',
#              'PLT/test/testFrameScanner.cpp',
#              'GUI/testGuiFrame.cpp',
#              'GUI/testGuiFrameBitmap.cpp',
#              'GUI/testGui.cpp',
#              'GUI/testGuiApp.cpp',
#              'GUI/testGuiHelloWorld.cpp',
#              'SND/testSND_Expr.cpp',
#              'SND/testSND_Sine.cpp',
#              'TRM/testTerminal.cpp',
#              'TRM/testTerminalFrame.cpp',
#              'TRM/testTerminalConsole.cpp',
#              'TRM/testTerminalApp.cpp',
#              'TRM/testTerminalLauncher.cpp',
#              'STB/testHelloWorld.cpp',
#              'STB/testConsoleApp.cpp',
#              'STB/testMidiFile.cpp',
