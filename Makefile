
targets = native # rp2040 rp2350

all: $(targets)

.PHONY: template
template:
	scripts/INSTALL_TEMPLATE.sh

.PHONY: docs
docs:
	doxygen scripts/Doxyfile

include Platform/build.make
