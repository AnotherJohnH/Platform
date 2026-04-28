#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(${PDK_TARGET})

# This warning seems broken in gcc 10 and later
target_compile_options(TGT PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Wno-stringop-overflow>")
target_compile_options(TGT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Wno-stringop-overflow>")

target_compile_definitions(TGT PUBLIC PDK_VERSION="${PROJECT_VERSION}")
target_compile_definitions(TGT PUBLIC PDK_COMMIT="${PDK_COMMIT}")
target_compile_definitions(TGT PUBLIC PDK_TARGET="${PDK_TARGET}")
target_compile_definitions(TGT PUBLIC PDK_MACHINE="${PDK_MACHINE}")
