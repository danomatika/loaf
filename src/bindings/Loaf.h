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

// global loaf lua bindings
namespace loaf {

/// \section Util

/// enable/disable internal loaf verbose printing
static void setVerbose(bool verbose) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->setVerbose(verbose);
}

/// print an OSC message and it's arguments to the console
static void printMessage(const ofxOscMessage &message, bool details=false) {
	cout << message.getAddress();
	if(details) {
		cout << " from " << message.getRemoteIp() << " "
		     << (int) message.getRemotePort() << endl;
	}
	stringstream line;
	for(int i = 0; i< message.getNumArgs(); ++i) {
		if(details) {
			line << "  " << message.getArgTypeName(i);
		}
		else {
			line << " ";
		}
		switch(message.getArgType(i)) {
			case OFXOSC_TYPE_INT32:
				line << (details ? ": " : "") << message.getArgAsInt32(i);
				break;
			case OFXOSC_TYPE_INT64:
				line << (details ? ": " : "") << message.getArgAsInt64(i);
				break;
			case OFXOSC_TYPE_FLOAT:
				line << (details ? ": " : "") << message.getArgAsFloat(i);
				break;
			case OFXOSC_TYPE_DOUBLE:
				line << (details ? ": " : "") << message.getArgAsDouble(i);
				break;
			case OFXOSC_TYPE_STRING:
				line << (details ? ": " : "") << message.getArgAsString(i);
				break;
			case OFXOSC_TYPE_SYMBOL:
				line << (details ? ": " : "") << message.getArgAsSymbol(i);
				break;
			case OFXOSC_TYPE_CHAR:
				line << (details ? ": " : "") << message.getArgAsChar(i);
				break;
			case OFXOSC_TYPE_MIDI_MESSAGE:
				line << (details ? ": " : "") << ofToHex(message.getArgAsMidiMessage(i));
				break;
			case OFXOSC_TYPE_RGBA_COLOR:
				line << (details ? ": " : "") << ofToHex(message.getArgAsRgbaColor(i));
				break;
			case OFXOSC_TYPE_TRUE:
			case OFXOSC_TYPE_FALSE:
			case OFXOSC_TYPE_NONE:
			case OFXOSC_TYPE_TRIGGER:
			case OFXOSC_TYPE_TIMETAG:
			case OFXOSC_TYPE_BLOB:
			case OFXOSC_TYPE_BUNDLE:
				break;
			default:
				continue;
		}
		cout << line.str() << endl;
		line.str("");
	}
}

/// \section OSC Server

/// start the OSC server thread
static void startListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->listener.start();
}

/// stop the OSC server thread
static void stopListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	app->listener.stop();
}

/// is the OSC server thread listening?
static bool isListening() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->listener.isListening();
}

/// set the OSC server port, restart server if running
static void setListenPort(int port) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	OscReceiver &listener = app->listener;
	if(listener.getPort() == port) {
		return; // silently ignore
	}
	listener.setPort(port);
}

/// get the OSC server port
static int getListenPort() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->listener.getPort();
}

/// \section Osc Sender

/// set the OSC send host ip/name
static void setSendHost(string host) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	OscSender &sender = app->sender;
	if(sender.getHost() == host) {
		return; // silently ignore
	}
	sender.setHost(host);
}

/// get the OSC send host ip/name
static string getSendHost() {
	ofApp *app = (ofApp *)ofGetAppPtr();
	return app->sender.getHost();
}

/// set the OSC send port
static void setSendPort(int port) {
	ofApp *app = (ofApp *)ofGetAppPtr();
	OscSender &sender = app->sender;
	if(sender.getPort() == port) {
		return; // silently ignore
	}
	sender.setPort(port);
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
