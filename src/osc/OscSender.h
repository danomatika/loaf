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

#include "ofxOscSender.h"
#include "config.h"

/// override to allow access to send settings
class OscSender : public ofxOscSender {

	public:
	
		void set(const string & host, int port) {
			h = host;
			p = port;
			ofxOscSender::setup(h, p);
		}
	
		void setHost(const string & host) {
			if(h == host) {
				return; // silently ignore
			}
			h = host;
			ofxOscSender::setup(h, p);
			ofLogVerbose(PACKAGE) << "send host: " << host;
		}

		string getHost() {
			return h;
		}
	
		void setPort(int port) {
			if(port < 1024) {
				ofLogWarning(PACKAGE) << "port must be > 1024";
				return;
			}
			if(p == port) {
				return; // silently ignore
			}
			p = port;
			ofxOscSender::setup(h, p);
			ofLogVerbose(PACKAGE) << "send port: " << port;
		}
		
		int getPort() {
			return p;
		}
	
	protected:
	
		// duplicate settings since ofxOscSender settings are private
		string h; // hostname
		int p;    // port
};
