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

# scons configuration for MBED LPC11U24 builds

set(prefix  arm-none-eabi-)
set(machine armv6m)
set(chip    LPC11U24)

#-------------------------------------------------------------------------------

set(CMAKE_ASM_COMPILER      ${prefix}as)
set(CMAKE_C_COMPILER        ${prefix}gcc)
set(CMAKE_CXX_COMPILER      ${prefix}g++)
set(CMAKE_AR                ${prefix}ar)
set(CMAKE_RANLIB            ${prefix}ranlib)
set(CMAKE_OBJCOPY           ${prefix}objcopy)
set(CMAKE_OBJDUMP           ${prefix}objdump)
set(CMAKE_SIZE              ${prefix}size)
set(CMAKE_C_LINK_EXECUTABLE ${prefix}ld)

#-------------------------------------------------------------------------------

add_compile_options(-std=c++11)
add_compile_options(-DNDEBUG)
add_compile_options(-DNCONSOLE)
add_compile_options(-DSMALL_MEMORY)
add_compile_options(-DNO_RTTI)
add_compile_options(-mcpu=cortex-m0)
add_compile_options(-mthumb)
add_compile_options(-mfloat-abi=soft)
add_compile_options(-fno-common)
add_compile_options(-fno-builtin)
add_compile_options(-fmessage-length=0)
add_compile_options(-fno-default-inline)
add_compile_options(-fno-exceptions)
add_compile_options(-fno-rtti)
add_compile_options(-ffunction-sections)
add_compile_options(-fdata-sections)

#-------------------------------------------------------------------------------

set(CMAKE_ASM_COMPILER        ${prefix}as)
set(CMAKE_C_COMPILER          ${prefix}gcc)
set(CMAKE_CXX_COMPILER        ${prefix}g++)
set(CMAKE_AR                  ${prefix}ar)
set(CMAKE_RANLIB              ${prefix}ranlib)
set(CMAKE_OBJCOPY             ${prefix}objcopy)
set(CMAKE_OBJDUMP             ${prefix}objdump)
set(CMAKE_SIZE                ${prefix}size)
set(CMAKE_C_LINK_EXECUTABLE   "${prefix}ld <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

set(CMAKE_EXE_LINKER_FLAGS  --static)
set(CMAKE_EXECUTABLE_SUFFIX .axf)

set(CMAKE_ASM_FLAGS -mcpu=cortex-m0)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE   "-O3")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG   "-g -O0")

set(platform_source
    Source/PLT/${target}/platform.cpp
    Source/PLT/Stub/Audio.cpp
    Source/PLT/Stub/Event.cpp
    Source/PLT/Stub/Frame.cpp
    Source/PLT/Stub/Image.cpp
    Source/PLT/Stub/Bitmap.cpp
    Source/PLT/Stub/Midi.cpp
    Source/PLT/Stub/Sounder.cpp
    Source/PLT/Stub/Yield.cpp
    Source/PLT/Stub/Info.cpp
    Source/PLT/Stub/File.cpp)

set(platform_libs tinyc gcc)

set(tinyc USE)

#-------------------------------------------------------------------------------

execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=armv6-m
                OUTPUT_VARIABLE gcc_lib)

link_directories(${gcc_lib})

#env['startup'] = env.Object('../../MTL/chip/'+CHIP+'/startup.s')
#env['script']  = env.File('../../MTL/chip/'+CHIP+'/script.ld')

#-------------------------------------------------------------------------------
# Adapt the Program() builder to generate a raw binary and some debug info

#env['BUILDERS']['Program'].action += '$OBJCOPY -O binary $TARGET ${TARGET}.bin; '+ \
                                     #'$OBJDUMP -D $SOURCE > ${TARGET}.dbg; '+ \
                                     #'$SIZE $TARGET'
