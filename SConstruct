#-------------------------------------------------------------------------------
#  Copyright (c) 2017 John D. Haughton
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

app      = 'PlatformTest'
version  = '0.0.2'

plt_source = ['Source/PLT/test/testAudioOut.cpp',
              'Source/PLT/test/testAudioIn.cpp',
              'Source/PLT/test/testMidiIn.cpp',
              'Source/PLT/test/testCanvas.cpp',
              'Source/PLT/test/testEvent.cpp',
              'Source/PLT/test/testFrame.cpp',
              'Source/GUI/testGui.cpp',
              'Source/GUI/testGuiHelloWorld.cpp',
              'Source/SND/testSND_Expr.cpp',
              'Source/SND/testSND_Sine.cpp',
              'Source/TRM/testTerminalCanvas.cpp',
              'Source/TRM/testTerminalConsole.cpp',
              'Source/TRM/testTerminalApp.cpp',
              'Source/TRM/testTerminalLauncher.cpp',
              'Source/STB/testHelloWorld.cpp',
              'Source/STB/testConsoleApp.cpp',
              'Source/STB/testEndian.cpp']

mtl_source = ['Source/MTL/testDigital.cpp']

# Get a build environment
env,libs = SConscript('build.scons', ['app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-O3'])

# Generate final source list
if env['startup'] == []:
   source_list = plt_source
else:
   source_list = mtl_source

source_list += env['app_src']

# Builders
exe = []
for source in source_list:
   filename=source.rsplit("/", 1)[1]
   binary=filename.rsplit(".", 1)[0]
   exe += env.Program(target=binary, source=[env['startup'], source])

Depends(exe, libs)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        ['include', libs, env['startup'], env['script'], env['platform_files'], 'LICENSE'])
