#!/usr/bin/env python3

#-------------------------------------------------------------------------------
#  Copyright (c) 2020 John D. Haughton
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

import os 
import argparse

#-------------------------------------------------------------------------------
# Parse command line arguments

def parseArgs():

   parser = argparse.ArgumentParser(description='arcxx')

   parser.add_argument('-r' ,'--root', dest='root', metavar='DIR', type=str,
                       help='root path')

   parser.add_argument('-w', '--width', dest='width', metavar='N', type=int, default=16,
                       help='bytes per line in output')

   parser.add_argument('-o' ,'--out', dest='output', metavar='ARCHIVE', type=str, default='ArCxx.cpp',
                       help='output archive')

   parser.add_argument(dest='input', metavar='DIR', type=str,
                       help='input path')

   return parser.parse_args()

# Entry point
#-------------------------------------------------------------------------------

def encodeFile(out, root_path, full_path):
   ''' Translate a file into a C array of bytes '''

   name = root_path.replace('.', '_')
   name = name.replace('/', '_')

   out.write('')
   out.write(f'static const uint8_t {name}[] =\n')
   out.write('{')

   with open(full_path, "rb") as in_file:

      first  = True
      offset = 0

      while True:

          data = in_file.read(16)
          if len(data) == 0:
             break

          for byte in data:
             if first:
                first = False
             else:
                out.write(',')

             if (offset % args.width) == 0:
                out.write('\n  ')

             out.write(f' 0x{byte:02X}')
             offset += 1

   out.write('\n};\n')

   return { 'filename':root_path, 'name':name, 'size':offset }

#-------------------------------------------------------------------------------

def scan(out, root, path):
   ''' Scan for files and encode them '''

   table = []

   for name in os.listdir(path):
      root_path = root + '/' + name
      full_path = path + '/' + name

      if os.path.isdir(full_path):
         table += scan(out, root_path, full_path)
      else:
         table += [encodeFile(out, root_path, full_path)]

   return table

#-------------------------------------------------------------------------------

args = parseArgs()

with open(args.output, 'w') as out:

   out.write('// Auto-generated by arcxx\n')
   out.write('\n')
   out.write('#include "STB/ArCxx.h"\n')
   out.write('\n')

   table = scan(out, args.root, args.input)

   out.write('\n')
   out.write(f'const unsigned STB::ArCxx::num_entries = {len(table)};\n')
   out.write('\n')
   out.write('const STB::ArCxx::Entry STB::ArCxx::entry[] =\n')
   out.write('{\n')

   for entry in table:
      out.write('   {"' + entry['filename'] + '", ' + entry['name'] + ', ' + str(entry['size']) + ' },\n')

   out.write('};\n')
