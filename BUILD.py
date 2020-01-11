#!/usr/bin/python

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
import shutil
import argparse

#-------------------------------------------------------------------------------

parser = argparse.ArgumentParser(description='Build Manager')

parser.add_argument(dest='targets', metavar='TARGET', type=str, nargs='+',
                    help='A target to build')

parser.add_argument('-p', '--pull', dest='pull', action='store_true',
                    help='Pull latest source before build')

parser.add_argument('-s', '--spotless', dest='spotless', action='store_true',
                    help='Erase all build state and build again')

parser.add_argument('-c', '--clean', dest='clean', action='store_true',
                    help='Erase all build state')

args = parser.parse_args()

#-------------------------------------------------------------------------------

def clean(target):

   print("Clean '"+target+"'")

   build_dir="build_"+target

   if os.path.exists(build_dir):
      shutil.rmtree(build_dir)

#-------------------------------------------------------------------------------

def build(target):

   print("Build for '"+target+"'")

   build_dir="build_"+target

   if not os.path.exists(build_dir):
      os.mkdir(build_dir)
      os.chdir(build_dir)
      os.system("cmake .. -DPLT_TARGET="+target)
   else:
      os.chdir(build_dir)

   os.system("make")
   os.chdir("..")

#-------------------------------------------------------------------------------

if args.pull:
   os.system("git pull --rebase")

for target in args.targets:

   if args.spotless:
      clean(target)
      build(target)
   elif args.clean:
      clean(target)
   else:
      build(target)
