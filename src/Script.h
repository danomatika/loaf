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

#include "ofxLua.h"

class ofxOscMessage;

class Script : protected ofxLuaListener {

	public:

		Script();

		ofxLua lua; //< lua instance

		/// num ms after script error before reloading automatically,
		/// ignored if set to -1
		int errorReload = -1;

		/// exit the app on a script error? overrides errorReload if set
		bool errorExit = false;

		/// load a new script or folder with a main.lua file
		/// clears the current lua state && sets script args if non-null
		bool load(const string &path, const vector<string> *args=nullptr);
		
		/// exit, reinit the lua state, and reload the current script
		bool reload();
	
		/// clears the current lua state, calls scriptExit
		void clear();
	
		/// draw any error messages
		void draw();
	
		/// run a given string
		bool eval(const string &text, bool reload=false);

		/// set the mouseX & mouseY global values
		void setMouseGlobals(int x, int y);
		
		/// send an osc message to the lua script
		/// calls the oscReceived lua function
		void oscReceived(const ofxOscMessage& message);
	
		/// access to the current script absolute path
		void setCurrentScript(string script) {currentScript = script;}
		string getCurrentScript() {return currentScript;}
	
		/// returns true if a given path is a loadable script file
		/// or directory with a main.lua script
		static bool isLoadablePath(const string &path);
	
	protected:
	
		/// setup the lua state
		bool initState();
	
		// clear the lua state
		void clearState();
	
		/// lua error callback
		void errorReceived(string& msg);

		string currentScript = ""; //< absolute path to current script
		vector<string> arg; //< global "arg" table passed from commandline
		bool error = false; //< is there an error?
		vector<string> errorMsg; //< error message, separated by lines
		long reloadTimestamp = 0; //< auto reload timestamp
};
