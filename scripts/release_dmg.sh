#! /bin/bash
#
# builds loaf macOS release as a notarized DMG
#

# stop on errors
set -e

# codesigning identity
IDENTITY="Daniel Wilcox"

### main

cd $(dirname $0)/../
make dist-clean dist-clobber
make app dist-dmg mac.codesign.identity="$IDENTITY"
make dist-clean
