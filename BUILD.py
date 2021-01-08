#!/usr/bin/python3

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
# Parse command line arguments

def parseArgs():
   parser = argparse.ArgumentParser(description='Build Manager')

   parser.add_argument(dest='targets', metavar='TARGET', type=str, nargs='+',
                       help='A target to build')

   parser.add_argument('-p', '--pull', dest='pull', action='store_true',
                       help='Pull latest source before build')

   parser.add_argument('-P', '--platform-pull', dest='platform_pull', action='store_true',
                       help='Pull latest Platform source before build')

   parser.add_argument('-s', '--spotless', dest='spotless', action='store_true',
                       help='Erase all build state and build again')

   parser.add_argument('-c', '--clean', dest='clean', action='store_true',
                       help='Erase all build state')

   parser.add_argument('-d', '--debug', dest='debug', action='store_true',
                       help='Debug builds')

   return parser.parse_args()

#-------------------------------------------------------------------------------
# Removce all build state for the given target

def clean(target):

   print("Clean '"+target+"'")

   build_dir="build_"+target

   if os.path.exists(build_dir):
      shutil.rmtree(build_dir)

#-------------------------------------------------------------------------------
# Create a build for the given target

def build(target, cmake_opts):

   print("======================================================================")
   print("Build for '"+target+"'")

   build_dir="build_"+target

   if not os.path.exists(build_dir):
      os.mkdir(build_dir)
      os.chdir(build_dir)
      os.system("cmake .. "+cmake_opts+ " -DPLT_TARGET="+target)
   else:
      os.chdir(build_dir)

   if target == "Emscripten" or target == "WebAsm":
      #
      # An Emscripten or Web Assembly build
      #
      EMSDK_PATH=os.getenv("HOME")+"/OpenSource/emsdk_portable"
      print("EMSDK_PATH = "+EMSDK_PATH)
      print("----------------------------------------------------------------------")
      os.system("source "+EMSDK_PATH+"/emsdk_env.sh; make")
   else:
      print("----------------------------------------------------------------------")
      os.system("make -j")

   os.chdir("..")

#-------------------------------------------------------------------------------
# Entry point

cmake_opts = ""

args = parseArgs()

if args.platform_pull:
   os.chdir("Platform")
   os.system("git pull --rebase")
   os.chdir("..")

if args.pull:
   os.system("git pull --rebase")

if args.debug:
   cmake_opts += " -DCMAKE_BUILD_TYPE=Debug"

for target in args.targets:

   if args.spotless:
      clean(target)
      build(target, cmake_opts)
   elif args.clean:
      clean(target)
   else:
      build(target, cmake_opts)
