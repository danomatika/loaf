#! /bin/sh
#
# Xcode run script for macOS to install Syphon framework to .app bundle
#
# usage: APP
# ie. APP is path/to/OFApp.app
#
# if run outside of Xcode, the OF_PATH variable must be set:
# OF_PATH=../../ macos_install_syphon.sh bin/MyApp.app
#
# Dan Wilcox danomatika.com 2023

# stop on error
set -e

if [ "$1" = "" ] ; then
	echo "usage: path/to/Project.app"
	exit 1
fi

APP_PATH="$1"
APP_NAME="$(basename ${APP_PATH%.*})"

SRC="$OF_PATH/addons/ofxSyphon/libs/Syphon/lib/osx"
DEST="$APP_PATH/Contents/Frameworks"

echo "install framework to $APP_PATH $APP_NAME"

# copy dylibs to app bundle
rsync -aedl "$SRC"/Syphon.framework "$DEST"
