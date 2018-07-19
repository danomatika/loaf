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
#include "ofAppRunner.h"

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
		options = nullptr;
	}

	// setup graphics
	ofGLWindowSettings settings;  // 1024, 576 for widescreen
	settings.setSize(640, 480);
	settings.windowMode = OF_WINDOW;
	if(options && app->options->opengl != "") {
		int major = 0, minor = 0;
		vector<string> ver = ofSplitString(app->options->opengl, ".");
		if(ver.size() > 1) {
			major = ofToInt(ver[0]);
			minor = ofToInt(ver[1]);
		}
		// OpenGL versions run 1.1 - 4.5
		if(major > 0 && major < 5 &&  // 1.x - 4.x
		   minor > -1 && minor < 6) { // x.0 - x.5
			settings.setGLVersion(major, minor);
		}
		else {
			ofLogWarning(PACKAGE) << "ignoring invalid open gl version string: "
			                      << app->options->opengl;
		}
	}
	ofCreateWindow(settings);

	// main app loop
	ofRunApp(app);
	
	// done
	return EXIT_SUCCESS;
}
