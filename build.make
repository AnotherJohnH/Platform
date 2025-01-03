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

.PHONY: clean
clean:
	rm -rf build

.PHONY: debug
debug:
	Platform/scripts/build.py -s -d native

.PHONY: spotless
spotless:
	Platform/scripts/build.py -s $(targets)

.PHONY: test
test:
	Platform/scripts/build.py --test ${targets}

#-------------------------------------------------------------------------------
# Build rules for specific targets

.PHONY: native
native:
	Platform/scripts/build.py native

.PHONY: Emscripten
Emscripten:
	Platform/scripts/build.py Emscripten

.PHONY: microbit
microbit:
	Platform/scripts/build.py microbit

.PHONY: microbitV1
microbitV1:
	Platform/scripts/build.py microbitV1

.PHONY: microbitV1_32k
microbitV1_32k:
	Platform/scripts/build.py microbitV1_32k

.PHONY: microbitV2
microbitV2:
	Platform/scripts/build.py microbitV2

.PHONY: mbedLPC11U24
mbedLPC11U24:
	Platform/scripts/build.py mbedLPC11U24

.PHONY: mbedLPC1768
mbedLPC1768:
	Platform/scripts/build.py mbedLPC1768

.PHONY: LPC810
LPC810:
	Platform/scripts/build.py LPC810

.PHONY: LPC1114
LPC1114:
	Platform/scripts/build.py LPC1114

.PHONY: rpipico
rpipico:
	Platform/scripts/build.py rpipico

.PHONY: rpipico2
rpipico2:
	Platform/scripts/build.py rpipico2
