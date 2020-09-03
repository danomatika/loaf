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
#include "Util.h"

// macro for chdir() and getcwd() as Windows uses protected variants
#ifdef TARGET_WIN32
	#include <direct.h>
	#define CHDIR _chdir
	#define GETCWD _getcwd
#else
	#include <unistd.h>
	#define CHDIR chdir
	#define GETCWD getcwd
#endif

std::string Util::getCurrentDir() {
	char currentDir[1024];
	GETCWD(currentDir, 1024);
	return currentDir;
}

bool Util::setCurrentDir(const std::string dir) {
	return CHDIR(dir.c_str()) == 0;
}
