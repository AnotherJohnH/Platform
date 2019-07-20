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

# cmake configuration for Emscripten builds

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

add_compile_options(-std=c++11)
add_compile_options(-Wno-main)
add_compile_options(-DCO_OP_MULTI_TASK)
add_compile_options(-s)
add_compile_options(USE_SDL=2)

set(platform_source
    Platform/Source/PLT/SDL2/Audio.cpp
    Platform/Source/PLT/SDL2/Event.cpp
    Platform/Source/PLT/SDL2/Frame.cpp
    Platform/Source/PLT/SDL2/Image.cpp
    Platform/Source/PLT/Stub/Bitmap.cpp
    Platform/Source/PLT/Stub/Midi.cpp
    Platform/Source/PLT/Stub/Sounder.cpp
    Platform/Source/PLT/Stub/Yield.cpp
    Platform/Source/PLT/Stub/Info.cpp
    Platform/Source/PLT/POSIX/File.cpp)

set(machine embc)

set(CMAKE_C_COMPILER   emcc)
set(CMAKE_CXX_COMPILER emcc)
set(CMAKE_AR           emar)
set(CMAKE_RANLIB       emranlib)

#env['PROGPREFIX'] = 'docs/'+env['machine']+'/'

set(CMAKE_EXE_LINKER_FLAGS  "-s USE_SDL=2")
set(CMAKE_EXECUTABLE_SUFFIX .html)

