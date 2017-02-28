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
#include "ofApp.h"

int main(int argc, char *argv[]) {

	// create app before parsing
	ofPtr<ofApp> app = ofPtr<ofApp>(new ofApp);

	// parse the commandline
	CommandLine *options = new CommandLine;
	if(!options->parse(argc, argv)) {
		return EXIT_FAILURE;
	}
	if(options->changed) {
		app->options = options;
	}
	else {
		delete options;
	}

	// setup graphics
	ofSetupOpenGL(640, 480, OF_WINDOW); // 1024, 576 for widescreen

	// main app loop
	ofRunApp(app);
	
	// done
	return EXIT_SUCCESS;
}
