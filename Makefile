
targets = native rp2040 rp2350

all: $(targets)

.PHONY: template
template:
	@scripts/INSTALL_TEMPLATE.py

.PHONY: docs
docs:
	doxygen scripts/Doxyfile

include build.make
