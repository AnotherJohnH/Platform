#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(testSIG
                  testMain.cpp
                  testConv.cpp
                  testOsc.cpp
                  testEnv.cpp)

   target_link_libraries(testSIG SIG STB)

   add_test(NAME testSIG COMMAND testSIG )

endif()
