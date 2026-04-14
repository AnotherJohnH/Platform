#-------------------------------------------------------------------------------
# Copyright (c) 2021 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

.PHONY: clean
clean:
	rm -rf build

.PHONY: debug
debug:
	PDK/scripts/build.py -s -d native

.PHONY: spotless
spotless:
	PDK/scripts/build.py -s $(targets)

.PHONY: test
test:
	PDK/scripts/build.py --test native

#-------------------------------------------------------------------------------
# Build rules for specific targets

.PHONY: native
native:
	@PDK/scripts/build.py native

.PHONY: Emscripten
Emscripten:
	PDK/scripts/build.py Emscripten

.PHONY: LPC810
LPC810:
	@PDK/scripts/build.py LPC810

.PHONY: LPC1114
LPC1114:
	@PDK/scripts/build.py LPC1114

.PHONY: LPC11U24
LPC11U24:
	@PDK/scripts/build.py LPC11U24

.PHONY: LPC1768
LPC1768:
	@PDK/scripts/build.py LPC1768

.PHONY: nRF51
nRF51:
	@PDK/scripts/build.py nRF51

.PHONY: nRF52
nRF52:
	@PDK/scripts/build.py nRF52

.PHONY: rp2040
rp2040:
	@PDK/scripts/build.py rp2040

.PHONY: rp2350
rp2350:
	@PDK/scripts/build.py rp2350

.PHONY: tiny2350
tiny2350:
	@PDK/scripts/build.py tiny2350
