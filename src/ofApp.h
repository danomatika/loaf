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

#include "ofMain.h"

#include "config.h"
#include "Script.h"
#include "PathWatcher.h"
#include "ofxOscSender.h"
#include "ofxOscReceiver.h"
#include "CommandLine.h"

class ofApp : public ofBaseApp {

	public:
	
	/// \section ofApp
	
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	/// \section Utils
	
		/// set loaf verbosity
		void setVerbose(bool verbose);
	
		/// get loaf verbosity level
		bool isVerbose();
	
	/// \section Osc
	
		/// start listening for osc messages
		void startListening();
	
		/// stop listenig for osc messages
		void stopListening();
	
		/// set the listen port, checks for valid range
		void setListenPort(int port);
	
		/// set the sender host
		void setSendHost(const string &host);
	
		/// set the sender port, checks for valid range
		void setSendPort(int port);
	
		/// osc received callback
		void oscReceived(const ofxOscMessage &message);
	
	/// \section PathWatcher
	
		/// patch change callback
		void pathChanged(const PathWatcher::Event &event);
	
	/// \section Data
		
		CommandLine *options = nullptr; //< commandline options, only valid in setup()
		PathWatcher watcher; //< path change watcher
		Script script; //< script manager
		
		ofxOscSender sender; //< OSC message sender
		ofxOscReceiver listener; //< OSC message listener
		std::string baseAddress = BASE_ADDRESS; //< base OSC address for loaf control messages
	
	private:
	
		bool verbose = false; //< current verbosity
};
