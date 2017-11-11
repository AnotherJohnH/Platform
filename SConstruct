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

plt_binaries = ['testHelloWorld',
                'testAudioOut',
                'testAudioIn',
                'testMidiIn',
                'testCanvas',
                'testEvent',
                'testFrame',
                'testGui',
                'testGuiHelloWorld',
                'testConsoleApp',
                'testSND_Expr',
                'testSND_Sine',
                'testTerminalCanvas',
                'testTerminalConsole',
                'testTerminalApp',
                'testTerminalLauncher',
                'testEndian']

mtl_binaries = ['testDigital']

app      = 'PlatformTest'
version  = '0.0.2'

# Get a build environment
env,libs = SConscript('build.scons', ['app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-O3'])

if env['startup'] == []:
   binaries = plt_binaries
else:
   binaries = mtl_binaries

# Builders
exe = []
for binary in binaries:
   exe += env.Program(binary, [env['startup'], 'Source/test/'+binary+'.cpp'])

Depends(exe, libs)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        ['include', libs, env['startup'], env['script'], env['platform_files'], 'LICENSE'])
