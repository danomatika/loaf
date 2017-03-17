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
#include "OscReceiver.h"

#include "config.h"

//--------------------------------------------------------------
OscReceiver::OscReceiver() {
	receiver = ofPtr<ofxOscReceiver>();
}

//--------------------------------------------------------------
void OscReceiver::setPort(int port) {
	this->port = port;
	if(isListening()) {
		stop();
		start();
	}
}

//--------------------------------------------------------------
void OscReceiver::setCallback(std::function<void(const ofxOscMessage &message)> const &callback) {
	this->callback = callback;
}

//--------------------------------------------------------------
void OscReceiver::update() {
	if(receiver.get() != NULL) {
		while(receiver->hasWaitingMessages()) {
			ofxOscMessage message;
			if(receiver->getNextMessage(message)) {
				if(callback) {
					callback(message);
				}
			}
		}
	}
}

//--------------------------------------------------------------
bool OscReceiver::start() {
	if(receiver.get() != NULL) {
		return false; // silently ignore
	}
	receiver = ofPtr<ofxOscReceiver>(new ofxOscReceiver);
	if(receiver.get() == NULL) {
		ofLogWarning(PACKAGE) << "could not create listener thread";
		return false;
	}
	receiver->setup(port);
	return true;
}

//--------------------------------------------------------------
bool OscReceiver::stop() {
	if(receiver.get() == NULL) {
		return false;
	}
	receiver.reset();
	return true;
}

//--------------------------------------------------------------
bool OscReceiver::isListening() {
	return receiver.get() != NULL;
}

//--------------------------------------------------------------
int OscReceiver::getPort() {
	return port;
}
