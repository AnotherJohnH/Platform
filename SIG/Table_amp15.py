#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

# Attenuation table...
#   0    => is infinity   0.0
#   1    => is -60 dB
# 0x7FFF => is 0 dB       1.0

import math
import table

DB_RANGE = 60

def amp_fn(x):
   return math.pow(10, (DB_RANGE / 20) * (x - 1)) if x > 0 else 0

table.gen('amp15',
          func      = lambda i,x : amp_fn(x),
          typename  = 'float',
          log2_size = 15,
          fmt       = 'f',
          is_const  = False)
