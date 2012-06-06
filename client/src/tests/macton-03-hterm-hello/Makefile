CWD      := $(shell pwd)
PROJECT  := $(shell basename $(CWD))

all:
	@

.PHONY: install
install:
	@mkdir -p $(NATIVECOLORS_ROOT)/client/srv/www/$(PROJECT)
	cp index.html $(NATIVECOLORS_ROOT)/client/srv/www/$(PROJECT)/
	cp hello.js $(NATIVECOLORS_ROOT)/client/srv/www/$(PROJECT)/


.PHONY: clean
clean:
