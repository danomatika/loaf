# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=$(realpath ../../..)
endif

# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

##### Makefile-mac-dist.mk

ifeq ($(shell uname -s),Darwin)

# app name to build
mac.app.name = loaf

# pull version from define in config.h
mac.dist.version := $(shell grep VERSION src/config.h | awk '{gsub("\"",""); print $$3}')

# openFrameworks projects use the "APPNAME Release" and "APPNAME Debug" naming
mac.app.project.scheme = $(mac.app.name) Release

# include openFrameworks project data
mac.dist.include = README.md CHANGES.txt LICENSE.txt examples

# add link to /Applications in dmg
mac.dmg.appslink = true

# codesign identity, usually a Developer ID Application string
# REPLACE this with your own or set it via the commandline:
# make app dist-dmg mac.codesign.identity="Foo Bar Baz Developers"
#mac.codesign.identity = Foo

MACDISTHELPER_DIR=.
include $(MACDISTHELPER_DIR)/Makefile-mac-dist.mk

# rename to txt
postdistdir::
	cd $(mac.dist.subdir) && mv README.md README.txt

# override zip and dmg naming to include platform and arch,
# could include some logic to choose between current arch or "universal"
mac.dmg.name=$(mac.dist.name.version)-macos
mac.zip.name=$(mac.dist.name.version)-macos

endif
