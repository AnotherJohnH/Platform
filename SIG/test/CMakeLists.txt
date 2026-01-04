#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL native)

   add_executable(testSIG
                  testMain.cpp
                  testOsc.cpp)

   target_link_libraries(testSIG SIG STB)

   add_test(NAME testSIG COMMAND testSIG )

endif()
