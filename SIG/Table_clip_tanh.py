#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import sys
import table

LOG2_TABLE_SIZE = 10
TABLE_SIZE      = 1 << LOG2_TABLE_SIZE
SCALE           = 3.0

table.gen('clip_tanh',
          func      = lambda i,x : math.tanh(2 * SCALE * x - SCALE),
          typename  = 'float',
          log2_size = LOG2_TABLE_SIZE,
          fmt       = 'f',
          is_const  = False)
