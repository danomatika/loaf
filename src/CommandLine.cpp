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
#include "CommandLine.h"

#include "config.h"
#include "ofFileUtils.h"
#ifdef TARGET_OS_LINUX
	// these are set by X.h and clash with optionsparser types
	#undef None
	#undef Bool
#endif
#include "ofLog.h"
#include "Options.h"

bool CommandLine::parse(int argc, char **argv) {
	#ifdef TARGET_OS_MAC
		// override path name within Mac app bundle
		argv[0] = (char *)PACKAGE;
	#endif
	
	// option index enum
	enum optionNames {
		UNKNOWN,
		HELP,
		VERS,
		SENDHOST,
		SENDPORT,
		LISTENPORT,
		STARTLISTENING,
		FULLSCREEN,
		IGNORECHANGES,
		ERROREXIT,
		ERRORRELOAD,
		OPENGL,
		SYPHONNAME,
		SYPHON,
		VERBOSE
	};

	// option and usage print descriptors
	const option::Descriptor usage[] = {
		{UNKNOWN, 0, "", "", Options::Arg::Unknown, "Options:"},
		{HELP, 0, "h", "help", Options::Arg::None, "  -h, --help \tprint usage and exit"},
		{VERS, 0, "", "version", Options::Arg::None, "  --version \tprint version and exit"},
		{SENDHOST, 0, "a", "address", Options::Arg::NonEmpty, "  -a, --address \tOSC host address to send to (default: localhost)"},
		{SENDPORT, 0, "p", "port", Options::Arg::Integer, "  -p, --port \tOSC port to send to (default: 8880)"},
		{LISTENPORT, 0, "l", "listen", Options::Arg::Integer, "  -l, --listen \tOSC port to listen on (default: 9990)"},
		{STARTLISTENING, 0, "s", "start", Options::Arg::None, "  -s, --start \tstart listening for OSC messages"},
		{FULLSCREEN, 0, "f", "fullscreen", Options::Arg::None, "  -f, --fullscreen \tstart in fullscreen"},
		{IGNORECHANGES, 0, "i", "ignore", Options::Arg::None, "  -i, --ignore \tignore script changes"},
		{ERROREXIT, 0, "e", "exit", Options::Arg::None, "  -e, --exit \texit after script error"},
		{ERRORRELOAD, 0, "r", "reload", Options::Arg::Integer, "  -r, --reload \treload timeout in secs after a script error"},
		{OPENGL, 0, "", "gl", Options::Arg::NonEmpty, "  --gl \ttry to set gl version to use ie. \"4.1\""},
		{SYPHONNAME, 0, "", "syphon-name", Options::Arg::NonEmpty, "  --syphon-name \tSyphon server name (default: screen)"},
		{SYPHON, 0, "", "syphon", Options::Arg::None, "  --syphon \tstart streaming screen with Syphon (macOS only)"},
		{VERBOSE, 0, "v", "verbose", Options::Arg::None, "  -v, --verbose \tverbose printing"},
		{UNKNOWN, 0, "", "", Options::Arg::Unknown, "\nArguments:"},
		{UNKNOWN, 0, "", "", Options::Arg::None, "  PATH \toptional lua script or folder to run"},
		{UNKNOWN, 0, "", "", Options::Arg::Unknown, "  args... \targuments to pass to the lua script"},
		{0, 0, 0, 0, 0, 0}
	};

	// parse commandline
	Options options("  " DESCRIPTION);
	if(!options.parse(usage, argc, argv)) {
		return false;
	}
	if(options.isSet(HELP)) {
		options.printUsage(usage, "[PATH [args]]");
		return false;
	}
	if(options.isSet(VERS)) {
		std::cout << VERSION << std::endl;
		return false;
	}
	if(options.numArguments() > 0 && options.getArgumentString(0) != "") {
		// load the config file (if one exists)
		path = ofFilePath::getAbsolutePath(options.getArgumentString(0), false);
		if(!ofFile::doesFileExist(path)) {
			ofLogError(PACKAGE) << "path " << path << " does not exist";
			return false;
		}
		if(options.numArguments() > 1) { // script arguments to pass on
			for(int i = 1; i < options.numArguments(); ++i) {
				args.push_back(options.getArgumentString(i));
			}
		}
		changed = true;
	}
	if(options.isSet(SENDHOST))       {sendHost = options.getString(SENDHOST); changed = true;}
	if(options.isSet(SENDPORT))       {sendPort = options.getUInt(SENDPORT); changed = true;}
	if(options.isSet(LISTENPORT))     {listenPort = options.getUInt(LISTENPORT); changed = true;}
	if(options.isSet(STARTLISTENING)) {startListening = true; changed = true;}
	if(options.isSet(FULLSCREEN))     {fullscreen = true; changed = true;}
	if(options.isSet(IGNORECHANGES))  {ignoreChanges = true; changed = true;}
	if(options.isSet(ERROREXIT))      {errorExit = true; changed = true;}
	if(options.isSet(ERRORRELOAD))    {errorReload = options.getInt(ERRORRELOAD); changed = true;}
	if(options.isSet(OPENGL))         {opengl = options.getString(OPENGL); changed = true;}
	if(options.isSet(SYPHONNAME))     {syphonName = options.getString(SYPHONNAME); changed = true;}
	if(options.isSet(SYPHON))         {syphon = true; changed = true;}
	if(options.isSet(VERBOSE))        {verbose = true; changed = true;}
	return true;
}
