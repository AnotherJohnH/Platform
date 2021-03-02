#-------------------------------------------------------------------------------
#  Copyright (c) 2019-2021 John D. Haughton
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

# cmake configuration for Emscripten builds

set(PLT_machine embc)

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(PLT_c_flags   "-DCO_OP_MULTI_TASK -Wno-main -s USE_SDL=2")

set(PLT_cxx_flags "-std=c++11")

include_directories(Platform/include/MTL/board/stub)

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER   emcc)
set(CMAKE_CXX_COMPILER emcc)
set(CMAKE_AR           emar)
set(CMAKE_RANLIB       emranlib)

set(CMAKE_EXE_LINKER_FLAGS      "-s USE_SDL=2")
set(CMAKE_EXECUTABLE_SUFFIX_CXX .html)

# Prevent compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#-------------------------------------------------------------------------------
# Configuration for libPLT.a

set(PLT_source
    Platform/Source/PLT/SDL2/Audio.cpp
    Platform/Source/PLT/SDL2/Event.cpp
    Platform/Source/PLT/SDL2/Frame.cpp
    Platform/Source/PLT/SDL2/Image.cpp
    Platform/Source/PLT/SDL2/Bitmap.cpp
    Platform/Source/PLT/Stub/Midi.cpp
    Platform/Source/PLT/Stub/Sounder.cpp
    Platform/Source/PLT/Stub/Yield.cpp
    Platform/Source/PLT/Stub/Info.cpp
    Platform/Source/PLT/POSIX/File.cpp)
