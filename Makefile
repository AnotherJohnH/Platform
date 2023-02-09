
targets = native

all: $(targets)

.PHONY: template
template:
	scripts/INSTALL_TEMPLATE.sh

include Platform/build.make
