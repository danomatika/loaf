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

#include <vector>
#include <string>

/// commandline option parser and settings
class CommandLine {

	public:
	
		/// parse commandline args, returns true on success
		bool parse(int argc, char **argv);
	
		/// was an option or argument changed from its default?
		bool changed = false;
	
		/// options
		unsigned int sendPort = 0;
		std::string sendHost = "";
		unsigned int listenPort = 0;
		bool startListening = false;
		bool fullscreen = false;
		bool ignoreChanges = false;
		bool errorExit = false;
		int errorReload = -1;
		std::string opengl = "";
		bool verbose = false;
	
		/// arguments
		std::string path = ""; //< path to script or script directory
		std::vector<std::string> args; //< args to pass to script
};
