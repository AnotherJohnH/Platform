#-------------------------------------------------------------------------------
#  Copyright (c) 2022 John D. Haughton
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

find_package(SDL2 REQUIRED)

add_library(PLT STATIC
    Midi.cpp
    MIDIInterface.cpp
    ../SDL2/Audio.cpp
    ../SDL2/Event.cpp
    ../SDL2/Frame.cpp
    ../SDL2/Image.cpp
    ../SDL2/Bitmap.cpp
    ../Stub/Info.cpp
    ../Stub/Sounder.cpp
    ../POSIX/Yield.cpp
    ../POSIX/File.cpp
    ../POSIX/Rtc.cpp
    ../POSIX/Socket.cpp)

target_compile_definitions(PLT
    PUBLIC PLT_FRAME_BUFFERED)

target_include_directories(PLT
    PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(PLT
    PRIVATE ${SDL2_LIBRARIES}
    INTERFACE "-framework CoreMIDI" "-framework CoreFoundation")
