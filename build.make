#-------------------------------------------------------------------------------
# Copyright (c) 2021 John D. Haughton
# SPDX-License-Identifier: MIT
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
	Platform/scripts/build.py --test native

#-------------------------------------------------------------------------------
# Build rules for specific targets

.PHONY: native
native:
	@Platform/scripts/build.py native

.PHONY: Emscripten
Emscripten:
	Platform/scripts/build.py Emscripten

.PHONY: microbit
microbit:
	@Platform/scripts/build.py microbit

.PHONY: microbitV1
microbitV1:
	@Platform/scripts/build.py microbitV1

.PHONY: microbitV1_32k
microbitV1_32k:
	@Platform/scripts/build.py microbitV1_32k

.PHONY: microbitV2
microbitV2:
	@Platform/scripts/build.py microbitV2

.PHONY: LPC810
LPC810:
	@Platform/scripts/build.py LPC810

.PHONY: LPC1114
LPC1114:
	@Platform/scripts/build.py LPC1114

.PHONY: LPC11U24
LPC11U24:
	@Platform/scripts/build.py LPC11U24

.PHONY: LPC1768
LPC1768:
	@Platform/scripts/build.py LPC1768

.PHONY: rp2040
rp2040:
	@Platform/scripts/build.py rp2040

.PHONY: rp2350
rp2350:
	@Platform/scripts/build.py rp2350

.PHONY: tiny2350
tiny2350:
	@Platform/scripts/build.py tiny2350
