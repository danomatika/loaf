/*==============================================================================

	loaf: lua, osc, and openFrameworks

	Copyright (c) 2020 Dan Wilcox <danomatika@gmail.com>

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

#include <string>

#ifdef LOAF_USE_SYPHON
	#include "ofxSyphon.h"
#else
	class ofxSyphonServer;
#endif

/// basic ofxSyphonServer wrapper
class Syphon {

	public:

		/// returns true if compiled with Syphon support
		static bool supported();

		/// load Lua module
		static void luaopen(void *L);

		Syphon();
		virtual ~Syphon();

		/// start publishing screen
		void start();

		/// stop publishing screen
		void stop();

		/// is publishing screen?
		bool isPublishing();

		/// publish the current screen buffer
		void publish();

		/// set the server name
		///  note: this is not the application name, the full naming format is:
		///        "application name - server name"
		void setName(const std::string &name);

		/// get the server name
		std::string getName();

		/// get the underlying ofxSyphonServer instance
		ofxSyphonServer *getServer();

	protected:

		ofxSyphonServer *server = nullptr; //< Syphon server
		std::string name = "screen";       //< cached server name
};
