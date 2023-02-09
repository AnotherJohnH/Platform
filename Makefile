
targets = native

all: $(targets)

template:
	scripts/INSTALL_TEMPLATE.sh

include Platform/build.make
