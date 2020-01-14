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

# cmake configuration for MacOS builds

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(PLT_cxx_flags "-std=c++11")

# TODO
#if env['CC'] == 'clang':
#   env.Append(CXXFLAGS = ['-Wdocumentation'])

include_directories(Platform/include/MTL/board/stub)

#-------------------------------------------------------------------------------
# Configuration for libPLT.a

set(PLT_source
    Platform/Source/PLT/target/native/platform.cpp
    Platform/Source/PLT/SDL2/Audio.cpp
    Platform/Source/PLT/SDL2/Event.cpp
    Platform/Source/PLT/SDL2/Frame.cpp
    Platform/Source/PLT/SDL2/Image.cpp
    Platform/Source/PLT/SDL2/Bitmap.cpp
    Platform/Source/PLT/Stub/Info.cpp
    Platform/Source/PLT/Stub/Sounder.cpp
    Platform/Source/PLT/target/macOS/Midi.cpp
    Platform/Source/PLT/POSIX/Yield.cpp
    Platform/Source/PLT/POSIX/File.cpp
    Platform/Source/PLT/POSIX/Rtc.cpp
    Platform/Source/PLT/POSIX/Socket.cpp)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set(PLT_libs ${SDL2_LIBRARIES})

# TODO
#env.Append(LINKFLAGS = ['-framework', 'CoreMIDI'])
#env.Append(LINKFLAGS = ['-framework', 'CoreFoundation'])

