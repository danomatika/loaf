/*
	loaf: lua, osc, and openFrameworks

	ofxSyphon bindings

	2020,2025 Dan Wilcox <danomatika@gmail.com>
*/

%module syphon
%{
#include "ofxSyphon.h"
%}

// STL types
%include <std_string.i>

// ----- Renaming -----

// strip "ofxSyphon" prefix from classes
%rename("%(strip:[ofxSyphon])s", %$isclass) "";

// strip "ofxSyphon" prefix from global functions & make first char lowercase
%rename("%(regex:/ofxSyphon(.*)/\\l\\1/)s", %$isfunction) "";

////////////////////////////////////////////////////////////////////////////////
// ----- BINDINGS --------------------------------------------------------------

// dummy typedef for ofxSyphonServerDirectoryEventArgs
class ofEventArgs {};

// deprecated
#ifndef OF_SWIG_DEPRECATED
%ignore ofxSyphonClient::bind;
%ignore ofxSyphonClient::unbind;
#endif

// ignore notification function which should not be called by users
%ignore handleNotification(const void *, void *);

// includes
%include "ofxSyphon/src/ofxSyphonClient.h"
%include "ofxSyphon/src/ofxSyphonServer.h"
%include "ofxSyphon/src/ofxSyphonServerDirectory.h"
