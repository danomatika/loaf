##### ofxSyphon
#
# note: PLATFORM_LIB_SUBPATH set by
# libs/openFrameworksCompiled/project/makefileCommon/config.share.mk
#

# additional build targets for macOS
ifeq ($(PLATFORM_LIB_SUBPATH),osx)

# path to ofxSyphon framework install script
SYPHON_INSTALL_SCRIPT=scripts/macos_install_syphon.sh

# build Debug app and install syphon framework
DebugSyphon: Debug
	OF_PATH=$(OF_ROOT) $(SYPHON_INSTALL_SCRIPT) `pwd`/bin/$(APPNAME)_debug.app

# build Release app and install syphon framework
ReleaseSyphon: Release
	OF_PATH=$(OF_ROOT) $(SYPHON_INSTALL_SCRIPT) `pwd`/bin/$(APPNAME).app

endif