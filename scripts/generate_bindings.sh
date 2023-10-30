#! /bin/sh
#
# generate Lua bindings
#

OF_DIR=../../../../

###

cd $(dirname $0)/../swig

make osc
make loaf

# generate syphon bindings only if the addon is present
if [ -d "$OF_DIR/addons/ofxSyphon" ] ; then
	make syphon
fi