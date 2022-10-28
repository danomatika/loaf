#! /bin/bash
#
# builds loaf macOS release as a notarized DMG
#

# stop on errors
set -e

if [ $# -lt 1 ] ; then
	echo "release_dmg: missing version string as argument"
	exit 1
fi
VERS=$1

# codesigning identity
IDENTITY="Daniel Wilcox"

### main

cd $(dirname $0)/../
make dist-clean dist-clobber
make app dist-dmg mac.codesign.identity="$IDENTITY"
make dist-clean
