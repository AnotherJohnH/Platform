#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import sys
import table

SAMPLE_RATE = float(sys.argv[1])

def index2freq(index, freq_frac_bits):
   """ Convert table index to freq (Hz) """

   A4_FREQ              = 440   # Freq of the A above middle C (Hz)
   MIDI_A4_NOTE         = 69    # MIDI note value for the A above middle C
   SEMITONES_PER_OCTAVE = 12

   scale = 1 << freq_frac_bits
   exp   = (index - (MIDI_A4_NOTE * scale)) / (SEMITONES_PER_OCTAVE * scale)
   freq  = A4_FREQ * math.pow(2, exp)
   return freq

def delta_fn32(i, _):
   """ Convert table index to a .32-bit frequency ratio """
   FREQ_FRAC_BITS = 7
   ratio = index2freq(i, FREQ_FRAC_BITS) / SAMPLE_RATE
   return int(ratio * (1 << 32) + 0.5)

table.gen('delta14_7',
          func      = delta_fn32,
          typename  = 'uint32_t',
          log2_size = 14,
          fmt       = table.hex_fmt(32),
          is_const  = False)
