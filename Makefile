
targets = native # rpipico2 rpipico

all: $(targets)

.PHONY: template
template:
	scripts/INSTALL_TEMPLATE.sh

.PHONY: docs
docs:
	doxygen scripts/Doxyfile

include Platform/build.make
