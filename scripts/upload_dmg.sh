#! /bin/bash
#
# uploads loaf macOS release DMG
#

if [ $# -lt 1 ] ; then
	echo "upload_dmg: missing version string as argument"
	exit 1
fi

VERS=$1
DMG=loaf-$VERS-macos.dmg

###

cd $(dirname $0)/../

scp $DMG danomatika@danomatika.com:/home/danomatika/danomatika.com/docs/releases/loaf/
