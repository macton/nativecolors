# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

CWD      := $(shell pwd)
PROJECT  := $(shell basename $(CWD))

all:
	@

.PHONY: clean
	@

.PHONY: install
install:
	@mkdir -p $(NATIVECOLORS_ROOT)/client/srv/www/usr/www/$(PROJECT)
	cp index.html $(NATIVECOLORS_ROOT)/client/srv/www/usr/www/$(PROJECT)
	cp -R js $(NATIVECOLORS_ROOT)/client/srv/www/usr/www/$(PROJECT)
