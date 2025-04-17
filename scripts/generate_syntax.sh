#! /bin/sh
#
# generate Lua bindings
#

ADDONS=../../../../addons
DEST=../doc/modules

# path to lua_syntax script
LUA=$ADDONS/ofxLua
LUA_SYNTAX=$LUA/scripts/lua_syntax.py

###

cd $(dirname $0)

cd $LUA/swig && make openFrameworks-symbols && make glm-symbols && cd -
mv $LUA/swig/*_lua_symbols.txt ../swig

cd ../swig
make symbols TARGET=osc INTERFACE=osc.i
make symbols TARGET=loaf INTERFACE=loaf.i

$LUA_SYNTAX of of_lua_symbols.txt
$LUA_SYNTAX glm glm_lua_symbols.txt
$LUA_SYNTAX osc osc_symbols.txt
$LUA_SYNTAX loaf loaf_symbols.txt

if [ -d $ADDONS/ofxSyphon ] ; then
	make symbols TARGET=syphon INTERFACE=syphon.i
	$LUA_SYNTAX syphon syphon_symbols.txt
else
	ehco "ofxSyphon not found, skipping..."
fi

# added by loaf.i
echo "loaf.send" >> loaf_syntax.txt

mkdir -p $DEST
cp -v *_syntax.txt $DEST
rm -f *_symbols.txt *_syntax.txt
