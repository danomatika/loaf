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
#include "ofApp.h"

#include "Loaf.h"

// OF APP

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	// OSC defaults
	sender.setup(SEND_HOST, SEND_PORT);
	ofxOscReceiverSettings settings;
	settings.port = LISTEN_PORT;
	settings.start = false; // manual start
	listener.setup(settings);
	
	// apply any commandline options
	bool watch = true;
	if(options) {
		if(options->verbose) {
			setVerbose(options->verbose);
		}
		if(options->fullscreen) {
			ofSetFullscreen(options->fullscreen);
			ofLogVerbose(PACKAGE) << "starting fullscreen";
		}
		if(options->ignoreChanges) {
			watch = false;
			ofLogVerbose(PACKAGE) << "ignoring script changes";
		}
		if(options->errorExit) {
			script.errorExit = options->errorExit;
			ofLogVerbose(PACKAGE) << "exit after a script error";
		}
		else if(options->errorReload > -1) {
			script.errorReload = options->errorReload * 1000; // s to ms
			ofLogVerbose(PACKAGE) << "reload " << options->errorReload
			                      << "s after a script error";
		}
		
		// try loading bundled script, otherwise a script set via commandline
		if(!loadBundledScript()) {
			if(options->path != "") {
				script.load(options->path, &options->args);
				watcher.addPath(options->path);
			}
		}
		
		// the following should override script
		if(options->sendHost != "") {
			setSendHost(options->sendHost);
		}
		if(options->sendPort > 0) {
			setSendPort(options->sendPort);
		}
		if(options->listenPort > 0) {
			setListenPort(options->listenPort);
		}
		if(options->startListening) {
			startListening();
		}
		if(options->syphonName != "") {
			syphon.setName(options->syphonName);
		}
		if(options->syphon) {
			syphon.start();
		}

		// cleanup
		delete options;
		options = nullptr;
	}
	else {
		// try loading bundled script
		loadBundledScript();
	}
	
	// path watching
	if(watch) {
		watcher.start();
	}

	// print OF binding and current opengl versions
	ofLogVerbose(PACKAGE) << "openframeworks version: "
	                      << OF_VERSION_MAJOR << "."
	                      << OF_VERSION_MINOR << "."
	                      << OF_VERSION_PATCH;
	ofLogVerbose(PACKAGE) << "open gl version: " << glGetString(GL_VERSION);
	
	// print the current OSC communication settings
	ofLogVerbose(PACKAGE) << "send host: " << sender.getSettings().host;
	ofLogVerbose(PACKAGE) << "send port: " << sender.getSettings().port;
	ofLogVerbose(PACKAGE) << "listen port: " << listener.getPort();
	if(listener.isListening()) {
		ofLogVerbose(PACKAGE) << "started listening";
	}
	if(Syphon::supported()) {
		ofLogVerbose(PACKAGE) << "syphon name: " << syphon.getName();
		if(syphon.isPublishing()) {
			ofLogVerbose(PACKAGE) << "started syphon";
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	// process received OSC events
	while(listener.hasWaitingMessages()) {
		ofxOscMessage message;
		if(listener.getNextMessage(message)) {
			oscReceived(message);
		}
	}
	
	// process any change events
	while(watcher.waitingEvents()) {
		pathChanged(watcher.nextEvent());
	}

	/// update the script
	script.update();
	script.lua.scriptUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {
	script.lua.scriptDraw();
	syphon.publish();
	script.drawError();
}

//--------------------------------------------------------------
void ofApp::exit() {
	script.lua.scriptExit();
	listener.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// handle special key events
	if(defaultKeys && ofGetKeyPressed(loaf::modKey)) {
		switch(key) {
			case 'f': case 6:
				ofToggleFullscreen();
				return;
			case 'r': case 18:
				if(!bundled) script.reload();
				return;
			case 'c': case 3:
				if(!bundled) script.clear();
				return;
		}
	}

	// pass everything else to lua
	script.lua.scriptKeyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	script.lua.scriptKeyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	script.setMouseGlobals(x, y);
	script.lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	script.setMouseGlobals(x, y);
	script.lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	script.setMouseGlobals(x, y);
	script.lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	script.setMouseGlobals(x, y);
	script.lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	script.lua.scriptMouseEntered(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
	script.lua.scriptMouseExited(x, y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	script.lua.scriptWindowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	std::string path = dragInfo.files[0];
	if(!bundled && Script::isLoadablePath(path)) {
		script.load(path);
		watcher.removeAllPaths();
		watcher.addPath(path);
	}
	else {
		script.lua.scriptDragEvent(dragInfo);
	}
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
	script.lua.scriptGotMessage(msg);
}

// BUNDLED SCRIPT

//--------------------------------------------------------------
std::string ofApp::bundledScriptPath() {
	#ifdef __APPLE__
		// .app bundle Contents/Resources/data
		return ofFilePath::getCurrentExeDir() + "../Resources/data/";
	#else
		// default data path
		return ofToDataPath("");
	#endif
}

//--------------------------------------------------------------
bool ofApp::isBundledScript() {
	return Script::isLoadablePath(ofApp::bundledScriptPath());
}

//--------------------------------------------------------------
bool ofApp::loadBundledScript(const std::vector<std::string> *args) {
	std::string path = ofApp::bundledScriptPath();
	if (Script::isLoadablePath(path) && script.load(path, args)) {
		ofLogVerbose(PACKAGE) << "found bundled project";
		bundled = true;
		return true;
	}
	return false;
}

// UTILS

//--------------------------------------------------------------
void ofApp::setVerbose(bool verbose) {
	if(verbose) {
		ofSetLogLevel(PACKAGE, OF_LOG_VERBOSE);
		ofLogVerbose(PACKAGE) << "verbose " << (verbose ? "on" : "off");
	}
	else {
		ofLogVerbose(PACKAGE) << "verbose " << (verbose ? "on" : "off");
		ofSetLogLevel(PACKAGE, OF_LOG_NOTICE);
	}
	this->verbose = verbose;
}

//--------------------------------------------------------------
bool ofApp::isVerbose() {
	return verbose;
}

//--------------------------------------------------------------
void ofApp::setUseDefaultKeys(bool defaultKeys) {
	ofLogVerbose(PACKAGE) << "use default keys " << (defaultKeys ? "on" : "off");
	this->defaultKeys = defaultKeys;
}

//--------------------------------------------------------------
bool ofApp::useDefaultKeys() {
	return defaultKeys;
}

//--------------------------------------------------------------
bool ofApp::isBundled() {
	return bundled;
}

// OSC

//--------------------------------------------------------------
void ofApp::startListening() {
	bool wasListening = listener.isListening();
	listener.start();
	if(!wasListening && !options) {
		ofLogVerbose(PACKAGE) << "started listening";
	}
}

//--------------------------------------------------------------
void ofApp::stopListening() {
	bool wasListening = listener.isListening();
	listener.stop();
	if(wasListening && !options) {
		ofLogVerbose(PACKAGE) << "stopped listening";
	}
}

//--------------------------------------------------------------
void ofApp::setListenPort(int port) {
	if(port < 1024) {
		ofLogWarning(PACKAGE) << "listen port must be > 1024";
		return;
	}
	if(listener.getPort() == port) {
		return; // silently ignore
	}
	ofxOscReceiverSettings settings = listener.getSettings();
	settings.port = port;
	listener.setup(settings);
	if(!options) {
		ofLogVerbose(PACKAGE) << "listen port: " << port;
	}
}

//--------------------------------------------------------------
void ofApp::setSendHost(const std::string &host) {
	if(sender.getHost() == host) {
		return; // silently ignore
	}
	ofxOscSenderSettings settings = sender.getSettings();
	settings.host = host;
	sender.setup(settings);
	if(!options) {
		ofLogVerbose(PACKAGE) << "send host: " << host;
	}
}

//--------------------------------------------------------------
void ofApp::setSendPort(int port) {
	if(port < 1024) {
		ofLogWarning(PACKAGE) << "send port must be > 1024";
		return;
	}
	if(sender.getPort() == port) {
		return; // silently ignore
	}
	ofxOscSenderSettings settings = sender.getSettings();
	settings.port = port;
	sender.setup(settings);
	if(!options) {
		ofLogVerbose(PACKAGE) << "send port: " << port;
	}
}

//--------------------------------------------------------------
void ofApp::oscReceived(const ofxOscMessage &message) {
	if(message.getAddress() == baseAddress + "/load") {
		if(message.getNumArgs() > 0 && message.getArgType(0) == OFXOSC_TYPE_STRING) {
			std::vector<std::string> args;
			for(int i = 1; i < message.getNumArgs(); ++i) {
				switch(message.getArgType(i)) {
					case OFXOSC_TYPE_INT32:
						args.push_back(ofToString(message.getArgAsInt32(i)));
						break;
					case OFXOSC_TYPE_INT64:
						args.push_back(ofToString(message.getArgAsInt64(i)));
						break;
					case OFXOSC_TYPE_FLOAT:
						args.push_back(ofToString(message.getArgAsFloat(i)));
						break;
					case OFXOSC_TYPE_DOUBLE:
						args.push_back(ofToString(message.getArgAsDouble(i)));
						break;
					case OFXOSC_TYPE_STRING:
						args.push_back(message.getArgAsString(i));
						break;
					case OFXOSC_TYPE_SYMBOL:
						args.push_back(message.getArgAsSymbol(i));
						break;
					default:
						ofLogWarning(PACKAGE) << "dropping script arg of type: "
						                      << (char)message.getArgType(1);
						break;
				}
					
			}
			script.load(message.getArgAsString(0), &args);
		}
	}
	else if(message.getAddress() == baseAddress + "/reload") {
		script.reload();
	}
	else if(message.getAddress() == baseAddress + "/quit") {
		ofLogVerbose(PACKAGE) << "received quit message, exiting...";
		ofExit();
	}
	else {
		// forward message to lua
		script.oscReceived(message);
	}
}

// SYPHON

//--------------------------------------------------------------
void ofApp::startSyphon() {
	bool wasPublishing = syphon.isPublishing();
	syphon.start();
	if(!wasPublishing && !options) {
		ofLogVerbose(PACKAGE) << "started syphon";
	}
}

//--------------------------------------------------------------
void ofApp::stopSyphon() {
	bool wasPublishing = syphon.isPublishing();
	syphon.stop();
	if(wasPublishing && !options) {
		ofLogVerbose(PACKAGE) << "stopped syphon";
	}
}

//--------------------------------------------------------------
void ofApp::setSyphonName(const std::string &name) {
	if(syphon.getName() == name) {
		return; // silently ignore
	}
	syphon.setName(name);
	if(!options) {
		ofLogVerbose(PACKAGE) << "syphon name: " << name;
	}
}

// PATCH WATCHER

//--------------------------------------------------------------
void ofApp::pathChanged(const PathWatcher::Event &event) {
	switch(event.change) {
		case PathWatcher::CREATED:
			ofLogVerbose(PACKAGE) << "path created " << event.path;
			break;
		case PathWatcher::MODIFIED:
			ofLogVerbose(PACKAGE) << "path modified " << event.path;
			break;
		case PathWatcher::DELETED:
			ofLogVerbose(PACKAGE) << "path deleted " << event.path;
			return;
		default: // NONE
			return;
	}
	if(Script::isLoadablePath(event.path)) {
		script.load(event.path);
	}
}
