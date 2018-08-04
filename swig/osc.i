/*
	loaf: lua, osc, and openFrameworks

	ofxOsc bindings

	2016 Dan Wilcox <danomatika@gmail.com>
*/

%module osc
%{
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"
%}

%include <attribute.i>

// STL types
%include <std_string.i>

// SWIG doesn't understand C++ streams
%ignore operator <<;

// other types we need to know about
typedef int int32_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// SWIG needs to know about the deprecation macro
#define OF_DEPRECATED_MSG(m, f)

// ----- Renaming -----

// strip "ofxOsc" prefix from classes
%rename("%(strip:[ofxOsc])s", %$isclass) "";

// strip "ofxOsc" prefix from global functions & make first char lowercase
%rename("%(regex:/ofxOsc(.*)/\\l\\1/)s", %$isfunction) "";

// strip "OFXOSC_" from constants & enums
%rename("%(strip:[OFXOSC_])s", %$isconstant) "";
%rename("%(strip:[OFXOSC_])s", %$isenumitem) "";

////////////////////////////////////////////////////////////////////////////////
// ----- BINDINGS --------------------------------------------------------------

// dummy forward declare for ofxOscReceiver which inherits from OscPacketListener
%ignore osc::OscPacketListener; 
namespace osc {
	class OscPacketListener {};
}

// don't bind argument types since they are only used internally by ofxOscMessage
%ignore ofxOscArg;
%ignore ofxOscArgInt32;
%ignore ofxOscArgInt;
%ignore ofxOscArgInt64;
%ignore ofxOscArgFloat;
%ignore ofxOscArgDouble;
%ignore ofxOscArgString;
%ignore ofxOscArgSymbol;
%ignore ofxOscArgChar;
%ignore ofxOscArgMidiMessage;
%ignore ofxOscArgBool;
%ignore ofxOscArgNone;
%ignore ofxOscArgTrigger;
%ignore ofxOscArgTimetag;
%ignore ofxOscArgBlob;
%ignore ofxOscArgRgbaColor;

// deprecations
%ignore ofxOscReceiver::getNextMessage(ofxOscMessage *);

// convert returned enum values to lua strings since
// ofxOscArgType enums are stored as strings 
#ifdef SWIGLUA
%typemap(out) ofxOscArgType {
	switch((char)$1) {
		case 'i': lua_pushstring(L, "i"); break;
		case 'h': lua_pushstring(L, "h"); break;
		case 'f': lua_pushstring(L, "f"); break;
		case 'd': lua_pushstring(L, "d"); break;
		case 's': lua_pushstring(L, "s"); break;
		case 'S': lua_pushstring(L, "S"); break;
		case 'c': lua_pushstring(L, "c"); break;
		case 'm': lua_pushstring(L, "m"); break;
		case 'T': lua_pushstring(L, "T"); break;
		case 'F': lua_pushstring(L, "F"); break;
		case 'N': lua_pushstring(L, "N"); break;
		case 'I': lua_pushstring(L, "I"); break;
		case 't': lua_pushstring(L, "t"); break;
		case 'b': lua_pushstring(L, "b"); break;
		case 'r': lua_pushstring(L, "r"); break;
		default:  lua_pushstring(L, "\0"); break;
	}
	SWIG_arg++;
}
#endif

// includes
%include "ofxOsc/src/ofxOscArg.h"
%include "ofxOsc/src/ofxOscBundle.h"
%include "ofxOsc/src/ofxOscMessage.h"
%include "ofxOsc/src/ofxOscParameterSync.h"
%include "ofxOsc/src/ofxOscReceiver.h"
%include "ofxOsc/src/ofxOscSender.h"

#ifdef ATTRIBUTES

// convenience attributes
%attributestring(ofxOscMessage, std::string, address, getAddress, setAddress)
%attributestring(ofxOscMessage, std::string, remoteHost, getRemoteHost)
%attributestring(ofxOscMessage, std::string, typeString, getTypeString)
%attribute(ofxOscMessage, int, remotePort, getRemotePort)
%attribute(ofxOscMessage, int, numArgs, getNumArgs);

%attribute(ofxOscBundle, int, messageCount, getMessageCount);
%attribute(ofxOscBundle, int, bundleCount, getBundleCount);

%attribute(ofxOscReceiver, int, port, getPort);
%attribute(ofxOscReceiver, bool, listening, isListening);
%attribute(ofxOscReceiver, ofxOscReceiverSettings&, settings, getSettings)

%attributestring(ofxOscSender, std::string, host, getHost);
%attribute(ofxOscSender, int, port, getPort);
%attribute(ofxOscSender, ofxOscSenderSettings&, settings, getSettings)

#endif

%extend ofxOscMessage {

	// tostring method
	const char* __str__() {
		static char str[255]; // provide a valid return pointer
		std::stringstream stream;
		stream << (*$self);
		sprintf(str, "%.255s", stream.str().c_str()); // copy & restrict length
		return str;
	}
};

%extend ofxOscBundle {

	// tostring method
	const char* __str__() {
		static char str[255]; // provide a valid return pointer
		std::stringstream stream;
		stream << (*$self);
		sprintf(str, "%.255s", stream.str().c_str()); // copy & restrict length
		return str;
	}
}

%extend ofxOscReceiver {

	// tostring method
	const char* __str__() {
		static char str[255]; // provide a valid return pointer
		std::stringstream stream;
		stream << (*$self);
		sprintf(str, "%.255s", stream.str().c_str()); // copy & restrict length
		return str;
	}
}

%extend ofxOscSender {

	// tostring method
	const char* __str__() {
		static char str[255]; // provide a valid return pointer
		std::stringstream stream;
		stream << (*$self);
		sprintf(str, "%.255s", stream.str().c_str()); // copy & restrict length
		return str;
	}
}
