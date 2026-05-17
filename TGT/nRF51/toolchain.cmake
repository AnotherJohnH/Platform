#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for mictobit (V1) builds

set(PDK_TARGET  nRF51)
set(PDK_MACHINE armv6m)

if (NOT DEFINED PDK_RAM_SIZE)
   set(PDK_RAM_SIZE 16k)
endif()

set(pdk_prefix  arm-none-eabi-)

#-------------------------------------------------------------------------------
# Special flags for this platform

set(CMAKE_ASM_FLAGS "-mcpu=cortex-m0 -mfloat-abi=soft -mthumb")

set(CMAKE_C_FLAGS "${CMAKE_ASM_FLAGS} \
-fno-common -fno-builtin -fmessage-length=0 \
-fno-default-inline -fno-exceptions -ffunction-sections -fdata-sections \
-I${CMAKE_SOURCE_DIR}/PDK/UCL/include \
-I${CMAKE_SOURCE_DIR}/PDK/TNY/include")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -Wno-volatile")

set(pdk_ld_flags  "--static -T${CMAKE_SOURCE_DIR}/PDK/TGT/${PDK_TARGET}/script_${PDK_RAM_SIZE}.ld")

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_SYSTEM_NAME         Generic)
set(CMAKE_SYSTEM_PROCESSOR    ${PDK_MACHINE})

set(CMAKE_ASM_COMPILER        ${pdk_prefix}as)
set(CMAKE_C_COMPILER          ${pdk_prefix}gcc)
set(CMAKE_CXX_COMPILER        ${pdk_prefix}g++)
set(CMAKE_AR                  ${pdk_prefix}ar)
set(CMAKE_RANLIB              ${pdk_prefix}ranlib)
set(CMAKE_OBJCOPY             ${pdk_prefix}objcopy)
set(CMAKE_OBJDUMP             ${pdk_prefix}objdump)
set(CMAKE_SIZE                ${pdk_prefix}size)

set(CMAKE_C_LINK_EXECUTABLE
    "${pdk_prefix}ld ${pdk_ld_flags} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>; \
     ${CMAKE_OBJCOPY} -O ihex <TARGET> <TARGET>.hex; \
     ${CMAKE_SOURCE_DIR}/PDK/TGT/util/objdump.py -b ${CMAKE_OBJDUMP} <TARGET>; \
     ${CMAKE_SIZE} <TARGET>")

set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Prevent compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE     STATIC_LIBRARY)
