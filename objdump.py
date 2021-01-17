#!/usr/bin/python3
#
# This scripts exists because I/O re-direction under cmake seems problamatic
# and objdump has no write to file command line option

import argparse
import subprocess

#-------------------------------------------------------------------------------
# Parse command line arguments

def parseArgs():

   parser = argparse.ArgumentParser(description='Wrapper for objdump -d')

   parser.add_argument('-b' ,'--binary', dest='binary', metavar='BINARY', type=str,
                       help='objdump binary to use', default = 'objdump')

   parser.add_argument('-o' ,'--out', dest='output', metavar='FILE', type=str,
                       help='output file', default=None)

   parser.add_argument(dest='input', metavar='INPUT', type=str,
                       help='input file')

   return parser.parse_args()

#-------------------------------------------------------------------------------

cli_args = parseArgs()
argv     = [cli_args.binary, '-d', cli_args.input]
result   = subprocess.run(argv, capture_output = True, text = True)

output_file = cli_args.output if cli_args.output else cli_args.input + '.dis'

with open(output_file, 'w') as out:
   out.write(result.stdout)
