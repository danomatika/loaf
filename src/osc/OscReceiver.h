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

#include "ofxOsc.h"

/// a threaded osc server, set callback to handle incoming messages
class OscReceiver {

	public:

		OscReceiver();

		/// set the port number, restarts thread if running
		void setPort(int port);

		/// osc message callback
		///
		/// called within the osc server thread, so you will need
		/// to protect any shared resources with a mutex or atomics
		void setCallback(std::function<void(const ofxOscMessage &message)> const &callback);

		/// process any receieved messages and forward them to the callback function
		void update();

		/// start the listening thread, opens connection
		void start();

		/// stop the listening thread, closes connection
		void stop();

		/// is the thread running?
		bool isListening();

		/// get port num
		int getPort();

	protected:
	
		/// underlying thread start/stop functions
		bool _start();
		bool _stop();
		
		unsigned int port; //< listening port
		ofPtr<ofxOscReceiver> receiver; //< wrapper receiver instance
		bool running; //< is the thread running?
	
		/// change event callback function pointer
		std::function<void(const ofxOscMessage &message)> callback = nullptr;
};
