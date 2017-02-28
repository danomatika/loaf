/*==============================================================================

	Options.h

	Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>

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

==============================================================================*/
#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "optionparser.h"

/// \class Options
/// \brief convenience wrapper for The Lean Mean C++ Options Parser which adds type conversions
///
/// See The Lean Mean C++ Options Parser docs for details: http://optionparser.sourceforge.net
///
/// Example usage:
///
/*

#include "Options.h"

#define VERSION "1.2.3"

int main(int argc, char* argv[]) {

	std::string sendingIp = "127.0.0.1";
	int sendingPort = 8880;
	bool listeningMulticast = false;

    // option index enum
    enum optionNames {
    	UNKNOWN,
    	HELP,
    	VERS,
    	IP,
    	PORT,
    	MULTICAST
    };

    // option and usage print descriptors, note the use of the Options::Arg functions
    // which provide extended type checks
    const option::Descriptor usage[] = {
    	{UNKNOWN, 0, "", "", Options::Arg::Unknown, "Options:\n"},
    	{HELP, 0, "h", "help", Options::Arg::None, "  -h, --help \tPrint usage and exit"},
    	{VERS, 0, "", "version", Options::Arg::None, "  --version \tPrint version and exit"},
    	{IP, 0, "i", "ip", Options::Arg::NonEmpty, "  -i, --ip \tIP address to send to (default: 127.0.0.1)"},
    	{PORT, 0, "p", "port", Options::Arg::Integer, "  -p, --port \tPort to send to (default: 8880)"},
    	{MULTICAST, 0, "m", "multicast", Options::Arg::None, "  -m, --multicast \tMulticast listening group address (off by default)"},
        {UNKNOWN, 0, "", "", Options::Arg::Unknown, "\nArguments:"},
	    {UNKNOWN, 0, "", "", Options::Arg::None, "  FILE \tOptional XML config file"},
    	{0, 0, 0, 0, 0, 0}
    };
    
    // parse and check help/version options
    Options options("  example program which demos Options.h");
    if(!options.parse(usage, argc, argv)) {
    	return false;
    }
    if(options.isSet(HELP)) {
    	options.printUsage(usage, "[FILE]");
    	return false;
    }
    if(options.isSet(VERS)) {
    	std::cout << VERSION << std::endl;
    	return false;
    }
    
    // load the config file (if one exists)
    if(options.numArguments() > 0) {
        std::cout << "Loading " << options.getArgumentString(0) << std::endl;
    }

    // read option values if set
    if(options.isSet(IP))         {sendingIp = options.getString(IP);}
    if(options.isSet(PORT))       {sendingPort = options.getUInt(PORT);}
    if(options.isSet(MULTICAST))  {listeningMulticast = true;}

    std::cout << "IP: " << sendingIp << std::endl
              << "Port: " << sendingPort << std::endl
              << "Multicast?: " << listeningMulticast << std::endl;

    return 0;
}
*/
///
/// Example usage print:
///
/*
Usage: ./test [options] [FILE]

  example program which demos Options.h

Options:

  -h, --help      Print usage and exit
  --version       Print version and exit
  -i, --ip        IP address to send to (default: 127.0.0.1)
  -p, --port      Port to send to (default: 8880)
  -m, --multicast Multicast listening group address (off by default)

Arguments:
  FILE            Optional XML config file
*/
///
class Options {

	public:
	
		/// constructor
		/// description: optional string printed after Usage line
		Options(std::string description="") : description(description), name("name") {
			parser = NULL;
			options = NULL;
			buffer = NULL;
		}
	
		/// destructor
		virtual ~Options() {
			clear();
		}
	
		/// clear current parsed options
		void clear() {
			if(parser) {delete parser;}
			if(options) {delete [] options;}
			if(buffer) {delete [] buffer;}
			parser = NULL;
			options = NULL;
			buffer = NULL;
		}
	
		/// parse commandline arguments using a TLMC++OP Descriptors array
		/// returns true on success, false on parse error
		bool parse(const option::Descriptor usage[], int argc, char **argv) {
			clear();
			if(argc > 0) {
				name = argv[0];
				// skip program name argv[0] if present
				argc--;
				argv++;
			}
			option::Stats stats(usage, argc, argv);
			options = new option::Option[stats.options_max];
			buffer = new option::Option[stats.buffer_max];
			parser = new option::Parser(usage, argc, argv, options, buffer);
			if(parser->error()) { // print error on unknown options, parse errors, etc
				return false;
			}
			return true;
		}
	
	/// \section Read Option Values Via Index
	
		/// is an option set?
		/// use this to check option existence before getting a value
		bool isSet(unsigned int index) {
			return options[index];
		}
	
		/// convert option from a string
		/// boolean values are 1/0, yes/no, true/false
		/// note: does not check index bounds
		bool getBool(unsigned int index) {
			std::string arg = options[index].last()->arg;
			if(arg == "1" || arg == "yes" || arg == "true") {
				return true;
			}
			return false; // "0" || "no" || "false"
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		int getInt(unsigned int index) {
			int i = 0;
			std::istringstream stream(options[index].last()->arg);
			stream >> i;
			return i;
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		unsigned int getUInt(unsigned int index) {
			unsigned int ui = 0;
			std::istringstream stream(options[index].last()->arg);
			stream >> ui;
			return ui;
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		float getFloat(unsigned int index) {
			float f = 0.f;
			std::istringstream stream(options[index].last()->arg);
			stream >> f;
			return f;
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		double getDouble(unsigned int index) {
			double d = 0.0;
			std::istringstream stream(options[index].last()->arg);
			stream >> d;
			return d;
		}
	
		/// get option
		/// note: does not check index bounds
		std::string getString(unsigned int index) {
			return options[index].last()->arg;
		}
	
		/// get the low level TLMC++OP option
		/// note: does not check index bounds
		option::Option* getOption(unsigned int index) {
			return options[index];
		}
	
	/// \section Read Non-option Arguments
	
		/// get the number of non-option arguments
		unsigned int numArguments() {
			return parser->nonOptionsCount();
		}
	
		/// convert a non-option argument from a string,
		/// boolean values are 1/0, yes/no, true/false
		/// note: does not check index bounds
		int getArgumentBool(unsigned int index) {
			std::string arg = parser->nonOption(index);
			if(arg == "1" || arg == "yes" || arg == "true") {
				return true;
			}
			return false; // "0" || "no" || "false"
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		int getArgumentInt(unsigned int index) {
			int i = 0;
			std::istringstream stream(parser->nonOption(index));
			stream >> i;
			return i;
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		unsigned int getArgumentUInt(unsigned int index) {
			unsigned int ui = 0;
			std::istringstream stream(parser->nonOption(index));
			stream >> ui;
			return ui;
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		float getArgumentFloat(unsigned int index) {
			float f = 0.f;
			std::istringstream stream(parser->nonOption(index));
			stream >> f;
			return f;
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		double getArgumentDouble(unsigned int index) {
			double d = 0.0;
			std::istringstream stream(parser->nonOption(index));
			stream >> d;
			return d;
		}
	
		/// get a non-option argument
		/// note: does not check index bounds
		std::string getArgumentString(unsigned int index) {
			return parser->nonOption(index);
		}
	
	/// \section Util
	
		/// print the commandline args (for debugging)
		static void print(int argc, char **argv) {
			for(int i = 0; i < argc; ++i) {
				std::cout << argv[i];
				if(i < argc-1) {
					std::cout << " ";
				}
			}
			std::cout << std::endl;
		}
	
		/// print usage using a TLMC++OP Descriptor array (low level),
		/// use *after* parsing otherwise program name won't be set
		///
		/// optionally provide a short argument usage string to be printed after
		/// "Usage: prog_name [options"
		///
		/// note: is used automatically if there was a parse error
		/// note: ignores built in options ("Options:" print, help & version options)
		///       so you'll need to provide them manually in the array
		void printUsage(const option::Descriptor usage[], std::string argShortUsage="") {
			std::cout << "Usage: " << name << " [options]";
			if(argShortUsage != "") {
				std::cout << " " << argShortUsage;
			}
			std::cout << std::endl << std::endl;
			if(description != "") {
				std::cout << description << std::endl << std::endl;
			}
			option::printUsage(std::cout, usage);
		}
	
	/// \section Option Type Checks
	
		/// extended option argument type check class
		struct Arg : public option::Arg {

			static option::ArgStatus Unknown(const option::Option& option, bool msg) {
				if(msg) {
					std::cerr << "Unknown option \"" << option.name << "\"" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}

			static option::ArgStatus Required(const option::Option& option, bool msg) {
				if(option.arg != 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires an argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}

			static option::ArgStatus NonEmpty(const option::Option& option, bool msg) {
				if(option.arg != 0 && option.arg[0] != 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a non-empty argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}
			
			static option::ArgStatus Bool(const option::Option& option, bool msg) {
				if(option.arg != 0 && option.arg[0]  != 0) {
					std::string arg = option.arg;
					if(arg == "0" || arg == "1" || arg == "yes" || arg == "no" || arg == "true" || arg == "false") {
						return option::ARG_OK;
					}
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a boolean argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}

			static option::ArgStatus Integer(const option::Option& option, bool msg) {
				char *endptr = 0;
				if(option.arg != 0 && strtol(option.arg, &endptr, 0)){};
				if(endptr != option.arg && *endptr == 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a non-decimal number argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}
			
			static option::ArgStatus Decimal(const option::Option& option, bool msg) {
				char *endptr = 0;
				if(option.arg != 0 && strtod(option.arg, &endptr)){};
				if(endptr != option.arg && *endptr == 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a number argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}
		};
	
	protected:

		std::string description; //< optional description line
		std::string name; //< program name (aka arg 0), only set after parsing
	
		option::Parser *parser; //< option parser, not valid until after parsing
		option::Option *options; //< parsed options, arrayed by index
		option::Option *buffer; //< raw parsed option buffer
};
