#! /bin/bash
#
# generate Lua bindings
#

WD=$(dirname $0)

###

cd $WD/../swig

make osc ATTRIBUTES=false
make loaf ATTRIBUTES=false
