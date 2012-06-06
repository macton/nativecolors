all: 
	cp $(NATIVECOLORS_ROOT)/client/src/packages/hterm/js/hterm.js .
	patch < hterm.js.patch

.PHONY:clean
clean:
	rm -f hterm.js

.PHONY: install
install: 
	@mkdir -p $(NATIVECOLORS_ROOT)/client/srv/www/hterm
	cp -R $(NATIVECOLORS_ROOT)/client/src/packages/hterm/html  $(NATIVECOLORS_ROOT)/client/srv/www/hterm/
	cp -R $(NATIVECOLORS_ROOT)/client/src/packages/hterm/js    $(NATIVECOLORS_ROOT)/client/srv/www/hterm/
	cp -R $(NATIVECOLORS_ROOT)/client/src/packages/hterm/css   $(NATIVECOLORS_ROOT)/client/srv/www/hterm/
	cp -R $(NATIVECOLORS_ROOT)/client/src/packages/hterm/audio $(NATIVECOLORS_ROOT)/client/srv/www/
	cp hterm.js $(NATIVECOLORS_ROOT)/client/srv/www/hterm/js/
