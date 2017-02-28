/*
    loaf: lua, osc, and openFrameworks

    loaf bindings
    
	2016 Dan Wilcox <danomatika@gmail.com>
*/

%module loaf
%{
#include "Loaf.h"
%}

%include <attribute.i>

// STL types
%include <stl.i>
%include <std_string.i>

// types we need to know about
%import(module="osc") "osc.i"

////////////////////////////////////////////////////////////////////////////////
// ----- BINDINGS --------------------------------------------------------------

%include "Loaf.h"

////////////////////////////////////////////////////////////////////////////////
// ----- LUA -------------------------------------------------------------------
#ifdef SWIGLUA

%luacode %{
-- variable argument OSC sender
function loaf.send(address, ...)
    local msg = osc.Message()
    if type(address) == "string" then
        msg.address = address
    else
        print("send: error, first argument is not a string")
        return
    end
    for i,v in ipairs{...} do
        if type(v) == "number" then
            msg:addFloatArg(v)
        elseif type(v) == "string" then
            msg:addStringArg(v)
        elseif type(v) == "boolean" then
            msg:addBoolArg(v)
        else
            print("send: skipping " .. type(v) .. " argument")
        end
    end
    loaf.sendMessage(msg)
end
%}

#endif
