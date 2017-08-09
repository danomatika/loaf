#! /bin/bash
#
# builds loaf macOS release as a ZIP
#

# stop on errors
set -e

if [ $# -lt 1 ] ; then
	echo "release_zip: missing version string as argument"
	exit 1
fi

VERS=$1
TEMP=loaf-$VERS
ZIP=loaf-$VERS-macos.zip
SRC_DIR=..

WD=$(dirname $0)

###

cd $WD

mkdir -p $TEMP

# app
cp -Rv $SRC_DIR/bin/loaf.app $TEMP

# readme etc
cp -v $SRC_DIR/README.md $TEMP/README.txt
cp -v $SRC_DIR/CHANGES.txt $TEMP
cp -v $SRC_DIR/LICENSE.txt $TEMP

# samples & tests
cp -Rv $SRC_DIR/examples $TEMP

# zip
zip -r $ZIP $TEMP

# cleanup
rm -rf $TEMP
