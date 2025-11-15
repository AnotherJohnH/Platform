#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for RaspberryPi Pico builds

set(PLT_PREFIX  arm-none-eabi-)
set(PLT_MACHINE armv6m)

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(CMAKE_ASM_FLAGS "-mcpu=cortex-m0")

set(CMAKE_C_FLAGS "-DPLT_NCONSOLE -DPLT_SMALL_MEMORY -DMTL_RP2040 \
-mcpu=cortex-m0 -mthumb -mfloat-abi=soft \
-fno-common -fno-builtin -fmessage-length=0 \
-fno-default-inline -fno-exceptions -ffunction-sections -fdata-sections \
-Wno-volatile")

set(CMAKE_CXX_FLAGS "-DNO_RTTI -fno-rtti ${CMAKE_C_FLAGS}")

set(PLT_LD_FLAGS  "--static -T${CMAKE_SOURCE_DIR}/Platform/MTL/${PLT_TARGET}/target/script.ld")

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_SYSTEM_NAME         Generic)
set(CMAKE_SYSTEM_PROCESSOR    ${PLT_MACHINE})

set(CMAKE_ASM_COMPILER        ${PLT_PREFIX}as)
set(CMAKE_C_COMPILER          ${PLT_PREFIX}gcc)
set(CMAKE_CXX_COMPILER        ${PLT_PREFIX}g++)
set(CMAKE_AR                  ${PLT_PREFIX}ar)
set(CMAKE_RANLIB              ${PLT_PREFIX}ranlib)
set(CMAKE_OBJCOPY             ${PLT_PREFIX}objcopy)
set(CMAKE_OBJDUMP             ${PLT_PREFIX}objdump)
set(CMAKE_SIZE                ${PLT_PREFIX}size)

set(CMAKE_C_LINK_EXECUTABLE
    "${PLT_PREFIX}ld ${PLT_LD_FLAGS} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>; \
     ${CMAKE_OBJCOPY} -O ihex <TARGET> <TARGET>.hex; \
     ${CMAKE_SOURCE_DIR}/Platform/MTL/util/ihexToUF2.py <TARGET>.hex -f0xE48BFF56 -o<TARGET>.uf2; \
     ${CMAKE_SOURCE_DIR}/Platform/scripts/objdump.py -b ${CMAKE_OBJDUMP} <TARGET>; \
     ${CMAKE_SIZE} <TARGET>")

set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Prevent compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE     STATIC_LIBRARY)
