#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TGT STATIC
            boot2.s
            vector_table_core0.s
            vector_table_core1.s)

