all: 
	@

.PHONY:clean
clean:
	@

# Note: Installed in root /client/srv/www - not expected to be sync'd to local file system
#       ...since this is the thing that syncs everything else to the local file system
.PHONY: install
install: 
	@mkdir -p $(NATIVECOLORS_ROOT)/client/srv/www/game-asset-loader
	cp $(NATIVECOLORS_ROOT)/client/src/packages/game-asset-loader/*.js $(NATIVECOLORS_ROOT)/client/srv/www/game-asset-loader/
