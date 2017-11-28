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
#pragma mark once

#include "ofApp.h"
#include "config.h"

#ifdef TARGET_WIN32
	#include <Winsock2.h>
#else
	#include <unistd.h>
#endif

// global loaf lua bindings
namespace loaf {

/// \section Util

/// enable/disable internal loaf verbose printing
static void setVerbose(bool verbose) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setVerbose(verbose);
}

/// get loaf verbosity
static bool isVerbose() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->isVerbose();
}

/// \section OSC Server

/// start the OSC server thread
static void startListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->startListening();
}

/// stop the OSC server thread
static void stopListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->stopListening();
}

/// is the OSC server thread listening?
static bool isListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->listener.isListening();
}

/// set the OSC server port, restart server if running
static void setListenPort(int port) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setListenPort(port);
}

/// get the OSC server port
static int getListenPort() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->listener.getPort();
}

/// get this machine's hostname
static string getHostname() {
	static char hostname[256];
	if(gethostname(hostname, 256) == 0) {
		return string(hostname);
	}
	return "";
}

/// \section Osc Sender

/// set the OSC send host ip/name
static void setSendHost(string host) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setSendHost(host);
}

/// get the OSC send host ip/name
static string getSendHost() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->sender.getHost();
}

/// set the OSC send port
static void setSendPort(int port) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setSendPort(port);
}

/// get the OSC send port
static int getSendPort() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->sender.getPort();
}

/// \section OSC Send Helpers

/// send an OSC message using the built-in OSC sender
static void sendMessage(ofxOscMessage& message) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->sender.sendMessage(message, false);
}

/// send an OSC bundle using the built-in OSC sender
static void sendBundle(ofxOscBundle& bundle) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->sender.sendBundle(bundle);
}

/// send an OSC "bang" address-only message using the built-in OSC sender
static void sendBang(string address) {
	ofxOscMessage message;
	message.setAddress(address);
	sendMessage(message);
}

/// send an OSC float message using the built-in OSC sender
static void sendFloat(string address, float f) {
	ofxOscMessage message;
	message.setAddress(address);
	message.addFloatArg(f);
	sendMessage(message);
}

/// send an OSC int message using the built-in OSC sender
static void sendInt(string address, int i) {
	ofxOscMessage message;
	message.setAddress(address);
	message.addIntArg(i);
	sendMessage(message);
}

/// send an OSC string message using the built-in OSC sender
static void sendString(string address, string s) {
	ofxOscMessage message;
	message.setAddress(address);
	message.addStringArg(s);
	sendMessage(message);
}

} // namespace
