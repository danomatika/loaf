#! /bin/bash
#
# generate Lua bindings
#

WD=$(dirname $0)

DEST=../doc/modules

# path to lua_syntax script
LUA=../../../../addons/ofxLua
LUA_SYNTAX=$LUA/swig/scripts/lua_syntax.py

###

cd $WD

cd $LUA/swig && make symbols LANG=lua ATTRIBUTES=false && cd -
mv $LUA/swig/of_lua_symbols.txt ../swig

cd ../swig
make symbols TARGET=osc ATTRIBUTES=false
make symbols TARGET=loaf ATTRIBUTES=false

$LUA_SYNTAX of of_lua_symbols.txt
$LUA_SYNTAX osc osc_symbols.txt
$LUA_SYNTAX loaf loaf_symbols.txt
echo "loaf.send" >> loaf_syntax.txt

mkdir -p $DEST
cp -v *_syntax.txt $DEST
rm -f *_symbols.txt *_syntax.txt
