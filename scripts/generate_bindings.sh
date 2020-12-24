#! /bin/bash
#
# generate Lua bindings
#

OF_DIR=../../../../

WD=$(dirname $0)

###

cd $WD/../swig

make osc
make loaf

# generate syphon bindings only if the addon is present
if [ -d "$OF_DIR/addons/ofxSyphon" ] ; then
	make syphon
fi