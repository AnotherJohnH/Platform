#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(VEC OBJECT
            boot2.s
            vector_table_core0.s
            vector_table_core1.s)

