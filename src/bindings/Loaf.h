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
#pragma once

#include "ofApp.h"
#include "Util.h"
#include "config.h"

#ifdef TARGET_WIN32
	#include <Winsock2.h>
#else
	#include <unistd.h>
#endif

// silence unused function warnings in C++17+
#if __cplusplus >= 201500
	#define UNUSED [[maybe_unused]]
#else
	#define UNUSED
#endif

// global loaf lua bindings
namespace loaf {

/// \section Util

/// enable/disable internal loaf verbose printing
UNUSED static void setVerbose(bool verbose) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setVerbose(verbose);
}

/// get loaf verbosity
UNUSED static bool isVerbose() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->isVerbose();
}

/// enable/disable loaf default key bindings
UNUSED static void setUseDefaultKeys(bool defaultKeys) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setUseDefaultKeys(defaultKeys);
}

/// get loaf default key bindings
UNUSED static bool useDefaultKeys() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->useDefaultKeys();
}

/// get loaf bundled status
UNUSED static bool isBundled() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->isBundled();
}

/// get the directory the application was started from
UNUSED static std::string getStartDirectory() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->startDir;
}

/// get the current working directory, may be different from OF data path
UNUSED static std::string getCurrentDirectory() {
	return Util::getCurrentDir();
}

/// set the current working directory, does not change OF data path
UNUSED static bool setCurrentDirectory(std::string dir) {
	return Util::setCurrentDir(dir);
}

/// get the platform modifier key
#ifdef __APPLE__
static const int modKey = OF_KEY_SUPER;
#else
static const int modKey = OF_KEY_CONTROL;
#endif

/// \section Script control

/// reload the current script
UNUSED static void reloadScript() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->script.reload();
}

/// clear the current script
UNUSED static void clearScript() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->script.clear();
}

/// \section OSC Server

/// start the OSC server thread
UNUSED static void startListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->startListening();
}

/// stop the OSC server thread
UNUSED static void stopListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->stopListening();
}

/// is the OSC server thread listening?
UNUSED static bool isListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->listener.isListening();
}

/// set the OSC server port, restart server if running
UNUSED static void setListenPort(int port) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setListenPort(port);
}

/// get the OSC server port
UNUSED static int getListenPort() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->listener.getPort();
}

/// get this machine's hostname
UNUSED static std::string getHostname() {
	static char hostname[256];
	if(gethostname(hostname, 256) == 0) {
		return std::string(hostname);
	}
	return "";
}

/// \section Osc Sender

/// set the OSC send host ip/name
UNUSED static void setSendHost(std::string host) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setSendHost(host);
}

/// get the OSC send host ip/name
UNUSED static std::string getSendHost() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->sender.getHost();
}

/// set the OSC send port
UNUSED static void setSendPort(int port) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setSendPort(port);
}

/// get the OSC send port
UNUSED static int getSendPort() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->sender.getPort();
}

/// \section OSC Send Helpers

/// send an OSC message using the built-in OSC sender
UNUSED static void sendMessage(ofxOscMessage &message) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->sender.sendMessage(message, false);
}

/// send an OSC bundle using the built-in OSC sender
UNUSED static void sendBundle(ofxOscBundle &bundle) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->sender.sendBundle(bundle);
}

/// send an OSC "bang" address-only message using the built-in OSC sender
UNUSED static void sendBang(std::string address) {
	ofxOscMessage message;
	message.setAddress(address);
	sendMessage(message);
}

/// send an OSC float message using the built-in OSC sender
UNUSED static void sendFloat(std::string address, float f) {
	ofxOscMessage message;
	message.setAddress(address);
	message.addFloatArg(f);
	sendMessage(message);
}

/// send an OSC int message using the built-in OSC sender
UNUSED static void sendInt(std::string address, int i) {
	ofxOscMessage message;
	message.setAddress(address);
	message.addIntArg(i);
	sendMessage(message);
}

/// send an OSC string message using the built-in OSC sender
UNUSED static void sendString(std::string address, std::string s) {
	ofxOscMessage message;
	message.setAddress(address);
	message.addStringArg(s);
	sendMessage(message);
}

/// \section Syphon

/// start the Syphon server
UNUSED static void startSyphon() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->startSyphon();
}

/// stop the Syphon server
UNUSED static void stopSyphon() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->stopSyphon();
}

/// is the Syphon server publishing the screen?
UNUSED static bool isSyphonPublishing() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->syphon.isPublishing();
}

/// set the Syphon server name
UNUSED static void setSyphonName(std::string name) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setSyphonName(name);
}

/// get the Syphon server name
UNUSED static std::string getSyphonName() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->syphon.getName();
}

/// get the underlying Syphon server instance
UNUSED static ofxSyphonServer *getSyphonServer() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->syphon.getServer();
}

} // namespace
