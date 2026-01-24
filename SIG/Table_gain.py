#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import sys
import table

# Gain table...
# 0x0000 => is  -inf dB    0.0         Hard mute
# 0x0001 => is ~ -60 dB   ~0.001
# 0x2000 => is   -40 dB    0.01
# 0x4000 => is   -20 dB    0.1
# 0x6000 => is     0 dB    1.0
# 0x7FFF => is ~ +20 dB  ~10.0 

MAX_DB = +20
MIN_DB = -60

LOG2_TABLE_SIZE = int(sys.argv[1])                # 15
TABLE_SIZE      = 1 << LOG2_TABLE_SIZE            # 32768

DB_RANGE      = MAX_DB - MIN_DB                   # 80
DB_PER_STEP   = DB_RANGE / TABLE_SIZE             # 80 / 32767
INDEX_ZERO_DB = int(round(-MIN_DB / DB_PER_STEP)) # 0x6000

def fn(i):
   return math.pow(10, (i - INDEX_ZERO_DB) * DB_PER_STEP / 20.0) if i > 0 else 0.0

table.gen('gain',
          func      = lambda i,x : fn(i),
          typename  = 'float',
          log2_size = LOG2_TABLE_SIZE,
          fmt       = 'f',
          is_const  = False)
