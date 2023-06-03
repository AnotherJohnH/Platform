#!/usr/bin/env python3

#-------------------------------------------------------------------------------
#  Copyright (c) 2021 John D. Haughton
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

import argparse
import binascii
import sys

#-------------------------------------------------------------------------------

# NOTE: borrowed from official pico-sdk

def bitrev(x, width):
    return int("{:0{w}b}".format(x, w=width)[::-1], 2)

#-------------------------------------------------------------------------------
#  IHEX decode

# standard record types
IHEX_DATA           = 0x0
IHEX_EOF            = 0x1
IHEX_EXT_SEG_ADDR   = 0x2
IHEX_START_SEG_ADDR = 0x3
IHEX_EXT_LIN_ADDR   = 0x4
IHEX_START_LIN_ADDR = 0x5

def split(text):
   ''' Split ASCII hex to a list of bytes (big endian order) '''

   byte_list = []
   num_bytes = len(text) // 2

   for i in range(num_bytes):
      byte      =  int(text[i*2: 2 + i*2], 16)
      byte_list += [byte]

   return byte_list


def Record(type_, addr = 0, data = []):
   ''' Create an IHEX record '''
   return {'type' : type_, 'addr' : addr, 'data' : data}


def readIHEX(filename):
   ''' Read an IHEX file '''

   def decodeRecord(line):

      if line[0] != ':':
         raise Exception(f"Bad record missing ':'")

      byte_list = split(line[1:-1])
      checksum  = sum(byte_list) & 0xFF

      if checksum != 0x00:
         raise Exception(f"Checksum fail [{checksum:02X}]")

      return Record(type_ = byte_list[3],
                    addr  = (byte_list[1]<<8) + byte_list[2],
                    data  = byte_list[4:-1])

   record_list = []

   with open(filename) as f:

      line_no = 1
      for line in f:

         if len(line) > 1:
            try:
               record_list += [ decodeRecord(line) ]
            except Exception as message:
               raise Exception(f"{str(message)}, line {line_no}")

         line_no += 1

   last = record_list[-1]
   if last['type'] != IHEX_EOF:
      raise Exception("Missing end of file record")

   return record_list[:-1]

#-------------------------------------------------------------------------------
# Embed CRC at the end of the 1st block

def addCRC(image):
   ''' Write CRC to the last 4 bytes in the first 256 byte block '''

   POLYNOMIAL = bitrev(0x04c11db7, 32)
   crc        = 0xFFFFFFFF

   for i in range(0, 252):

      byte = bitrev(image[i], 8)

      byte = (byte ^ crc) & 0xff

      for bit in range(0, 8):
          if (byte & 1) != 0:
             byte = (byte >> 1) ^ POLYNOMIAL
          else:
             byte = byte >> 1

      crc = byte ^ (crc >> 8)

   crc = bitrev(crc, 32)

   print(f'CRC = {crc:08x}')

   image[252] = (crc >>  0) & 0xff
   image[253] = (crc >>  8) & 0xff
   image[254] = (crc >> 16) & 0xff
   image[255] = (crc >> 24) & 0xff

#-------------------------------------------------------------------------------
# UF2 encode

UF2_MAGIC1         = 0x0A324655
UF2_MAGIC2         = 0x9E5D5157
UF2_FLAG_FAMILY_ID = 0x00002000
UF2_FAMILY_RP2040  = 0xE48BFF56
UF2_FINAL_MAGIC    = 0x0AB16F30

def writeUF2(filename, family_id, address, data):
   ''' Write a UF2 file '''

   def writeRecord(f, family_id, address, data, block_no, num_blocks):

      def ltl32(record, offset, value):
         """ Encode 32-but value as 4 little endian bytes """
         record[offset + 0] = value & 0xFF
         record[offset + 1] = (value >>  8) & 0xFF
         record[offset + 2] = (value >> 16) & 0xFF
         record[offset + 3] = (value >> 24) & 0xFF

      # Build record
      record = [0] * 512
      ltl32(record,   0, UF2_MAGIC1)
      ltl32(record,   4, UF2_MAGIC2)
      ltl32(record,   8, UF2_FLAG_FAMILY_ID)
      ltl32(record,  12, address)
      ltl32(record,  16, block_size)
      ltl32(record,  20, block_no)
      ltl32(record,  24, num_blocks)
      ltl32(record,  28, family_id)
      record[32 : 32 + len(data)] = data
      ltl32(record, 508, UF2_FINAL_MAGIC)

      f.write(bytearray(record))

   block_size = 256

   with open(filename, 'wb') as f:

      block_no   = 0
      num_blocks = (len(data) + block_size) // block_size

      for block_no in range(num_blocks):
         offset = block_no * block_size
         writeRecord(f, family_id,
                     address, data[offset : offset + block_size],
                     block_no, num_blocks)
         address += block_size

#-------------------------------------------------------------------------------

def parseArgs():
   ''' Parse command line arguments '''

   parser = argparse.ArgumentParser(description='Create Microsoft UF2 file from ihex')

   parser.add_argument(dest='input', type=str, default=None,
                       help='input file', metavar='<ihex>')

   parser.add_argument('-o' ,'--out', dest='out', type=str, default='out.uf2',
                       help='output file', metavar='<uf2>')

   parser.add_argument('-f' ,'--family', dest='family_id', type=str,
                       default=UF2_FAMILY_RP2040,
                       help='UF2 family id', metavar='<family>')

   return parser.parse_args()

#-------------------------------------------------------------------------------

args = parseArgs()

records = readIHEX(args.input)

# Convert IHEX records to a raw image
address = 0
image   = []

for record in records:
   if record['type'] == IHEX_EXT_LIN_ADDR:
      address = (record['data'][0] << 24) | (record['data'][1] << 16)

   elif record['type'] == IHEX_DATA:
      image += record['data']

# Add in the CRCR32 required by the RP2040 boot ROM
addCRC(image)

writeUF2(args.out, args.family_id, address, image)
