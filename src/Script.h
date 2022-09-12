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

/// lua script state wrapper, handles calling scriptSetup and scriptExit
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
		bool load(const std::string &path, const std::vector<std::string> *args=nullptr);
		
		/// exit, reinit the lua state, and reload the current script
		bool reload();
	
		/// calls scriptExit and clears the current lua state
		void clear();

		/// handles auto reload and calls scriptSetup if needed
		void update();

		/// draw any error messages, does nothing if there is no error or the
		/// lua state is invalid
		void drawError();
	
		/// run a given string
		bool eval(const std::string &text, bool reload=false);

		/// set the mouseX & mouseY global values
		void setMouseGlobals(int x, int y);
		
		/// send an osc message to the lua script
		/// calls the oscReceived lua function
		void oscReceived(const ofxOscMessage &message);
	
		/// access to the current script absolute path
		void setCurrentScript(std::string script) {currentScript = script;}
		std::string getCurrentScript() {return currentScript;}
	
		/// returns true if a given path is a loadable script file
		/// or directory with a main.lua script
		static bool isLoadablePath(const std::string &path);
	
	protected:
	
		/// setup the lua state
		bool initState();
	
		// clear the lua state
		void clearState();
	
		/// lua error callback
		void errorReceived(std::string &message);

		std::string currentScript = ""; //< absolute path to current script
		std::vector<std::string> arg; //< global "arg" table passed from commandline
		bool setupNeeded = false; //< should scriptSetup be called?
		bool error = false; //< is there an error?
		bool errorDraw = false; //< draw the error? (start on the next frame)
		std::vector<std::string> errorMsg; //< error message, separated by lines
		long reloadTimestamp = 0; //< auto reload timestamp
};
