#! /bin/bash
#
# generate Lua bindings
#

WD=$(dirname $0)

DEST=../doc/modules

# path to lua_syntax script
LUA=../../../../addons/ofxLua
LUA_SYNTAX=$LUA/scripts/lua_syntax.py

###

cd $WD

cd $LUA/swig && make openFrameworks-symbols && make glm-symbols && cd -
mv $LUA/swig/*_lua_symbols.txt ../swig

cd ../swig
make symbols TARGET=osc
make symbols TARGET=loaf

$LUA_SYNTAX of of_lua_symbols.txt
$LUA_SYNTAX glm glm_lua_symbols.txt
$LUA_SYNTAX osc osc_symbols.txt
$LUA_SYNTAX loaf loaf_symbols.txt

# added by loaf.i
echo "loaf.send" >> loaf_syntax.txt

mkdir -p $DEST
cp -v *_syntax.txt $DEST
rm -f *_symbols.txt *_syntax.txt
