#! /bin/bash
#
# generate Lua bindings
#

WD=$(dirname $0)

###

cd $WD/../swig

make osc
make loaf
