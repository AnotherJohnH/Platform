#-------------------------------------------------------------------------------
# Copyright (c) 2020 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import argparse
import sys

#-------------------------------------------------------------------------------

def split(text):
   ''' Split ASCII hex to a list of bytes (big endian order) '''

   byte_list = []
   num_bytes = len(text) // 2

   for i in range(num_bytes):
      byte      =  int(text[i*2: 2 + i*2], 16)
      byte_list += [byte]

   return byte_list

#-------------------------------------------------------------------------------
# Record constructors

# standard record types
REC_DATA           = 0x0
REC_EOF            = 0x1
REC_EXT_SEG_ADDR   = 0x2
REC_START_SEG_ADDR = 0x3
REC_EXT_LIN_ADDR   = 0x4
REC_START_LIN_ADDR = 0x5

# microbit extended record types
REC_START_BLK      = 0xA
REC_END_BLK        = 0xB
REC_PAD_BLK        = 0xC
REC_V2_DATA        = 0xD
REC_EXTRA          = 0xE

PAD_BYTE              = 'FF'        # byte code used for padding
MAX_BYTES_PER_LINE    = 16          # maximum record byte count
CHUNK_SIZE            = 512         # file chunking size (file chars)
V1_CODE_BLOCK         = 0x9900C0DE  # block start type for V1 image
V2_CODE_BLOCK         = 0x9903C0DE  # block start type for V2 image
CHARS_IN_EMPTY_RECORD = 1+2+4+2+2+1 # Empty record :<SS><AAAA><TT><CC><eol>

def Record(type_, addr = 0, data = []):
   ''' Create an IHEX record '''
   return {'type' : type_, 'addr' : addr, 'data' : data}

def ExtendedLinearAddressRecord(ext_addr):
   assert ext_addr >= 0 and ext_addr <= 0xFFFF
   return Record(REC_EXT_LIN_ADDR, data = split(f'{ext_addr:04X}'))

def StartBlockRecord(block_type):
   assert block_type >= 0 and block_type <= 0xFFFFffff
   return Record(REC_START_BLK, data = split(f'{block_type:08X}'))

def PadRecord(size):
   assert size <= 255
   return Record(REC_PAD_BLK, data = split(PAD_BYTE * size))

def EndBlockRecord(size):
   assert size <= 255
   return Record(REC_END_BLK, data = split(PAD_BYTE * size))

#-------------------------------------------------------------------------------

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
   if last['type'] != REC_EOF:
      raise Exception("Missing end of file record")

   return record_list[:-1]

#-------------------------------------------------------------------------------

def writeIHEX(filename, record_list):
   ''' Write an IHEX file '''

   def writeRecord(f, record):
      byte_list = [ len(record['data']),
                    record['addr'] >> 8,
                    record['addr'] % 0x100,
                    record['type'] ]

      byte_list += record['data']

      checksum  =  (0x100 - sum(byte_list)) & 0xFF
      byte_list += [checksum]

      f.write(':')
      for byte in byte_list:
         f.write(f'{byte:02X}')
      f.write('\n')

   f = open(filename, 'w') if filename else sys.stdout

   for record in record_list:
      writeRecord(f, record)

   writeRecord(f, Record(REC_EOF))

#-------------------------------------------------------------------------------

def createUniversalIHEX(v1_records, v2_records):
   ''' Convert microbit V1 and V2 records to a microbit universal IHEX records 
       See https://github.com/microbit-foundation/spec-universal-hex/blob/master/docs/format.md
       Adds several new record types to the ihex format and an awkward file chunking scheme '''

   def endBlockWithPadding(record_list):

      # Calculate size of file so far
      size = 0
      for record in record_list:
         size += len(record['data']) * 2 + CHARS_IN_EMPTY_RECORD

      padding  = CHUNK_SIZE - (size % CHUNK_SIZE)
      end_size = 0

      while True:
         end_size = padding - CHARS_IN_EMPTY_RECORD
         if end_size >= 0 and end_size <= (MAX_BYTES_PER_LINE * 2):
            break

         pad_size = min(MAX_BYTES_PER_LINE * 2, end_size - CHARS_IN_EMPTY_RECORD)

         assert (pad_size % 2) == 0
         record_list += [PadRecord(pad_size // 2)]
         size        += pad_size + CHARS_IN_EMPTY_RECORD
         padding     =  CHUNK_SIZE - (size % CHUNK_SIZE)

      # Add end of block record
      assert (end_size % 2) == 0
      record_list += [EndBlockRecord(end_size // 2)]

      return record_list

   univ_records = []

   # V1 section header
   univ_records += [ExtendedLinearAddressRecord(0)]
   univ_records += [StartBlockRecord(V1_CODE_BLOCK)]

   # Copy in V1 records
   first = True
   for record in v1_records:

      # Only copy in Data and non-redundant Extended Linear Address records
      if record['type'] == REC_DATA or \
         record['type'] == REC_EXT_LIN_ADDR and (not first or (record != univ_records[0])):
         univ_records += [record]
      first = False

   univ_records = endBlockWithPadding(univ_records)

   # V2 section header
   univ_records += [ExtendedLinearAddressRecord(0)]
   univ_records += [StartBlockRecord(V2_CODE_BLOCK)]

   # Copy in V2 records
   for record in v2_records:
      # Change data record type for V2 image
      if record['type'] == REC_DATA:
         record['type'] = REC_V2_DATA
      univ_records += [record]

   univ_records = endBlockWithPadding(univ_records)

   return univ_records

#-------------------------------------------------------------------------------

def parseArgs():
   ''' Parse command line arguments '''

   parser = argparse.ArgumentParser(description='Create microbit universal IHEX file')

   parser.add_argument('-1' ,'--inV1', dest='in_v1', type=str, default=None,
                       help='micro:bit V1 file', metavar='<ihex>')

   parser.add_argument('-2' ,'--inV2', dest='in_v2', type=str, default=None,
                       help='micro:bit V2 file', metavar='<ihex>')

   parser.add_argument('-o' ,'--out', dest='out', type=str, default=None,
                       help='output file', metavar='<ihex>')

   return parser.parse_args()

#-------------------------------------------------------------------------------

args = parseArgs()

v1_records = readIHEX(args.in_v1) if args.in_v1 else []
v2_records = readIHEX(args.in_v2) if args.in_v2 else []

univ_records = createUniversalIHEX(v1_records, v2_records)

writeIHEX(args.out, univ_records)
