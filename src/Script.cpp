/*==============================================================================

	loaf: lua, osc, and openFrameworks
  
	Copyright (c) 2016 Dan Wilcox <danomatika@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/danomatika/loaf for documentation

==============================================================================*/
#include "Script.h"

#include "config.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofLight.h"
#include "ofFileUtils.h"
#include "ofxOsc.h"
#include "Util.h"

// declare the wrapped modules
extern "C" {
	int luaopen_osc(lua_State* L);
	int luaopen_loaf(lua_State* L);
}

//--------------------------------------------------------------
Script::Script() {
	lua.addListener(this);
}

//--------------------------------------------------------------
bool Script::load(const std::string &path, const std::vector<std::string> *args) {
	
	ofFile scriptOrFolder(path);
	std::string script, directory, name;
	
	// directory?
	if(scriptOrFolder.isDirectory()) {
		directory = scriptOrFolder.getAbsolutePath();
		script = ofFilePath::join(directory, "main.lua");
		if(!ofFile::doesFileExist(script)) {
			ofLogError(PACKAGE) << "given folder " << path << " missing a main.lua script";
			return false;
		}
		name = scriptOrFolder.getBaseName();
	}
	else { // or lua script file
		script = scriptOrFolder.getAbsolutePath();
		directory = ofFilePath::getEnclosingDirectory(script);
		if(scriptOrFolder.getExtension() != "lua") {
			ofLogError(PACKAGE) << "given script " << path << " is not a lua file";
			return false;
		}
		name = scriptOrFolder.getFileName();
	}
	
	// reload current script?
	if(script == currentScript) {
		return reload();
	}
	currentScript = script;
	ofLogVerbose(PACKAGE) << "loading " << name;
	
	// clear or set arg table
	if(args == nullptr) {
		arg.clear();
	}
	else {
		arg = (*args);
		ofLogVerbose(PACKAGE) << "script args: " << ofToString(arg);
	}
	
	// reset and doublecheck directory existence
	clear();
	if(!ofDirectory::doesDirectoryExist(directory)) {
		ofLogError(PACKAGE) << "script dir " << path << " does not exist";
		return false;
	}
	
	// change the current directory to the scene directory,
	// this allows the lua state to find local files
	if(!Util::setCurrentDir(directory.c_str())) {
		switch(errno) {
			case EACCES:
				ofLogError(PACKAGE) << "couldn't access dir " << directory;
				break;
			default:
				ofLogError(PACKAGE) << "couldn't change dir to " << directory
					<< ", error " << errno << " " << strerror(errno);
				break;
		}
		return false;
	}
	
	// set data path to script dir
	ofSetDataPathRoot(directory);
	
	// change the current dir to the scene directory,
	// this allows the lua state to find local files
	bool ret = lua.doScript(currentScript, true);
	if(ret) {
		setupNeeded = true;
		ofLogVerbose(PACKAGE) << "current dir now " << Util::getCurrentDir();
	}
	return ret;
}

//--------------------------------------------------------------
bool Script::reload() {
	if(currentScript == "") {
		return false;
	}
	ofLogVerbose(PACKAGE) << "reloading " << ofFilePath::getFileName(currentScript);
	clear();
	bool ret = lua.doScript(currentScript);
	if(ret) {
		setupNeeded = true;
	}
	return ret;
}

//--------------------------------------------------------------
void Script::clear() {
	lua.scriptExit();
	lua.clear();
	setupNeeded = false;
	error = false;
	errorMsg.clear();
	initState(); // stay inited
}

//--------------------------------------------------------------
void Script::update() {
	// handle auto reload
	if(error && errorReload > -1 &&
	   ofGetElapsedTimeMillis() - reloadTimestamp > errorReload) {
		reload();
	}
	// make sure setup is called within a GL context
	if(setupNeeded) {
		ofSetupGraphicDefaults();
		ofDisableLighting();
		ofSetBackgroundAuto(true);
		lua.scriptSetup();
		setupNeeded = false;
	}
}

//--------------------------------------------------------------
void Script::drawError() {
	if(error) {
		ofPushView();
		ofPushStyle();
		bool lighting = ofGetLightingEnabled();
		if(lighting) {ofDisableLighting();}
		ofSetRectMode(OF_RECTMODE_CORNER);
		int lineWidth = (ofGetWidth()/8) - 2;
		int x = 8, y = (ofGetHeight()/2) - errorMsg.size()*10; // 20/2 = 10
		for(int i = 0; i < errorMsg.size(); ++i) {
			for(int c = 0; c < errorMsg[i].length(); c += lineWidth) {
				ofDrawBitmapStringHighlight(
					errorMsg[i].substr(c, lineWidth), x, y,
					ofColor(255), ofColor(220, 20, 60)
				);
				y += 20;
			}
		}
		if(lighting) {ofEnableLighting();}
		ofPopStyle();
		ofPopView();
	}
}

//--------------------------------------------------------------
bool Script::eval(const std::string &text, bool reload) {
	if(reload) {
		lua.scriptExit();
		if(!initState()) {
			return false;
		}
	}
	if(!lua.doString(text)) {
		initState(); // reset
		return false;
	}
	if(reload) {
		setupNeeded = true;
	}
	return true;
}

//--------------------------------------------------------------
void Script::setMouseGlobals(int x, int y) {
	if(!lua.isValid()) {
		return;
	}
	lua_pushglobaltable(lua);
	lua_pushinteger(lua, x);
	lua_setfield(lua, -2, "mouseX");
	lua_pushinteger(lua, y);
	lua_setfield(lua, -2, "mouseY");
	lua_pop(lua, 1);
}

//--------------------------------------------------------------
void Script::oscReceived(const ofxOscMessage& message) {
	if(!lua.isValid() || !lua.isFunction("oscReceived")) {
		return;
	}
	lua_getglobal(lua, "oscReceived");
	lua.pushobject("ofxOscMessage", new ofxOscMessage(message));
	if(lua_pcall(lua, 1, 0, 0) != 0) {
		std::string line = "Error running oscReceived(): " + (std::string) lua_tostring(lua, -1);
		lua.errorOccurred(line);
	}
}

//--------------------------------------------------------------
bool Script::isLoadablePath(const std::string &path) {
	return ofFilePath::getFileExt(path) == "lua" ||
	       ofFile::doesFileExist(ofFilePath::join(path, "main.lua"));
}

// PROTECTED

//--------------------------------------------------------------
bool Script::initState() {
	if(!lua.init(true)) {
		ofLogError(PACKAGE) << "could not init lua";
		return false;
	}
	
	luaopen_osc(lua);  // osc bindings
	luaopen_loaf(lua); // loaf bindings
	
	// script arguments
	lua.newTable("arg");
	lua.pushTable("arg");
	lua.setString(0, currentScript); // arg[0]
	for(int i = 0; i < arg.size(); ++i) {
		lua.setString(i+1, arg[i]);
	}
	lua.popTable();
	
	// for processing heads
	setMouseGlobals(0, 0);

	return true;
}

//--------------------------------------------------------------
void Script::clearState() {
	lua.clear();
	arg.clear();
	currentScript = "";
	error = false;
	errorMsg.clear();
}

//--------------------------------------------------------------
void Script::errorReceived(std::string& msg) {
	ofLogError(PACKAGE) << msg;
	error = true;
	errorMsg.clear();
	errorMsg = ofSplitString(msg, "\n");
	if(errorExit) {
		OF_EXIT_APP(1);
	}
	if(errorReload > -1) {
		reloadTimestamp = ofGetElapsedTimeMillis();
	}
}
