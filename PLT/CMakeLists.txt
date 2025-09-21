#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(${PLT_TARGET})

target_compile_definitions(PLT PUBLIC PLT_TARGET="${PLT_TARGET}")
target_compile_definitions(PLT PUBLIC PLT_VERSION="${PROJECT_VERSION}")
target_compile_definitions(PLT PUBLIC PLT_MACHINE="${PLT_MACHINE}")
target_compile_definitions(PLT PUBLIC PLT_COMMIT="${PLT_COMMIT}")

target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Wall>")
target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Werror>")

target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Wall>")
target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Werror>")

target_include_directories(PLT PUBLIC ${PLT_TARGET} ..)

add_subdirectory(test)
