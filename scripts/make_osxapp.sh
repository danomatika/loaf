#! /bin/sh
#
# Bundles a loaf project into a macOS .app bundle.
#
# This basically uses a given loaf.app and modifies it by changing the
# Info.plist and copying the project resources into the bundle.
#
# Dan Wilcox danomatika.com
#

# stop on error
set -e
#set -x

verbose=
version="0.1.0"
copystring=""
bundleid=""
category="developer-tools"
plistfile=""
iconfile=""
LOAF=/Applications/loaf.app

# PlistBuddy command for editing app bundle Info.plist from template
PLIST_BUDDY=/usr/libexec/PlistBuddy

# Functions
# ---------------------------------------------------------

CWD="$(pwd)"

makeabsolute() {
    if [ "$1" != "${1#/}" ] ; then
        # absolute
        echo "$1"
    else
        # relative
        echo "$CWD/$1"
    fi
}

# Help message
#----------------------------------------------------------
help() {
echo -e "
Usage: make_osxapp.sh [OPTIONS] SRC NAME

  Makes a loaf project .app bundle for macOS using a loaf.app wrapper

Options:
  -h,--help             display this help message

  --verbose             verbose copy prints

  -l,--loaf APP         path to loaf.app, default: /Applications/loaf.app

  -v,--version VER      version string, ex. ##.##.##

  -s,--copystring STR   basic info string displayed by Finder,
                        ex. \"appname: an interactive doodad widget\"

  -b,--bundleid ID      bundle identifier, ex. \"com.company.appname\"

  -c,--category CAT     launch services category type, ex. \"music\", \"games\",
                        \"developer-tools\", \"video\", \"utilities\", etc

  -p,--plistfile FILE   bundle Info.plist file, supersedes -vsbc

  -i,--iconfile FILE    bundle icon file

Arguments:

  SRC              required source path to copy into bundle, ie. the loaf
                   project directory containing the top-level main.lua script
                   and associated scripts and resources

  NAME             required bundle application name, ie. \"App Name\"

"
}

# Parse command line arguments
#----------------------------------------------------------
while [ "$1" != "" ] ; do
    case $1 in
        -v|--version)
            shift 1
            if [ $# == 0 ] ; then
                echo "-v,--version option requires a VER argument"
                exit 1
            fi
            version=$1
            ;;
        -l|--loaf)
            shift 1
            if [ $# == 0 ] ; then
                echo "-l,--loafapp option requires an APP argument"
                exit 1
            fi
            LOAF=${1%/} # remove trailing slash
            ;;
        -s|--copystring)
            shift 1
            if [ $# == 0 ] ; then
                echo "-s,--copystring option requires a STR argument"
                exit 1
            fi
            copystring=$1
            ;;
        -b|--bundleid)
            shift 1
            if [ $# == 0 ] ; then
                echo "-b,--bundleid option requires an ID argument"
                exit 1
            fi
            bundleid=$1
            ;;
        -c|--category)
            shift 1
            if [ $# == 0 ] ; then
                echo "-c,--category option requires a CAT argument"
                exit 1
            fi
            category=$1
            ;;
        -p|--plistfile)
            shift 1
            if [ $# == 0 ] ; then
                echo "-p,--plistfile option requires a FILE argument"
                exit 1
            fi
            plistfile=$1
            ;;
        -i|--iconfile)
            shift 1
            if [ $# == 0 ] ; then
                echo "-i,--iconfile option requires a FILE argument"
                exit 1
            fi
            iconfile="$(makeabsolute $1)"
            ;;
        --verbose)
            verbose=-v
            ;;
        -h|--help)
            help
            exit 0
            ;;
        *)
            break ;;
    esac
    shift 1
done

# check for source path
if [ "$1" != "" ] ; then
    SRC="$(makeabsolute $1)"
else
    echo "SRC argument required"
    exit 1
fi
echo "$SRC"

# check for name argument and set app path in the dir the script is run from
if [ "$2" != "" ] ; then
    NAME="$2"
    
else
    echo "NAME argument required"
    exit 1
fi
APP="$(makeabsolute ${NAME}.app)"
if [ "$bundleid" == "" ] ; then
    bundleid="com.loaf.${NAME% /-}"
fi

# Go
#----------------------------------------------------------

# change to the dir of this script
cd $(dirname $0)

# app bundle destination path
DEST="$APP/Contents/Resources"

# remove old app if found
if [ -d "$APP" ] ; then
    rm -rf "$APP"
fi

if [ "$verbose" != "" ] ; then
    echo "==== Creating $APP"
fi

# get absolute path in original location and make local copy
cd - > /dev/null # quiet
LOAF="$(cd "$(dirname "$LOAF")"; pwd)/$(basename "$LOAF")"
cd - > /dev/null # quiet
if [ ! -e "$LOAF" ] ; then
    echo "$LOAF not found"
    exit 1
fi
if [ "$verbose" != "" ] ; then
    echo "==== Using $LOAF"
fi
cp -R $verbose "$LOAF" .

# change app name and rename resources
mv $(basename "$LOAF") "$APP"
chmod -R u+w "$APP"
if [ -f "$APP/Contents/MacOS/loaf" ] ; then
    mv "$APP/Contents/MacOS/loaf" "$APP/Contents/MacOS/${NAME}"
else
    echo "$LOAF is not a valid loaf .app bundle"
    rm -rf "$APP"
    exit 1
fi

# prepare bundle resources
mkdir -p $verbose "$DEST/data"
cp -R $verbose "$SRC"/* "$DEST/data"
if [ "$iconfile" != "" ] ; then
    cp $verbose "$iconfile" "$DEST/icon.icns"
fi

# update info plist
INFO_PLIST="$APP/Contents/Info.plist"
if [ -f "$plistfile" ] ; then
    # simply copy supplied plist in
    cp -R $verbose "$plistfile" "$INFO_PLIST"
else
    # set version identifiers & contextual strings in Info.plist
    $PLIST_BUDDY -c "Set:CFBundleExecutable \"$NAME\"" "$INFO_PLIST"
    $PLIST_BUDDY -c "Set:CFBundleName \"$NAME\"" "$INFO_PLIST"
    $PLIST_BUDDY -c "Set:CFBundleDisplayName \"$NAME\"" "$INFO_PLIST"
    $PLIST_BUDDY -c "Set:CFBundleIdentifier \"$bundleid\"" "$INFO_PLIST"
    $PLIST_BUDDY -c "Set:CFBundleVersion \"$version\"" "$INFO_PLIST"
    $PLIST_BUDDY -c "Set:CFBundleShortVersionString \"$version\"" "$INFO_PLIST"
    $PLIST_BUDDY -c "Set:LSApplicationCategoryType \"public.app-category.${category}\"" "$INFO_PLIST"
    if [ "$copystring" != "" ] ; then
       $PLIST_BUDDY -c "Add:NSHumanReadableCopyright string \"$copystring\"" "$INFO_PLIST"
    fi
fi

# re-sign as ad-hoc
if [ "$verbose" != "" ] ; then
    echo "==== Signing $APP"
fi
codesign $verbose --force --deep -s "-" "$APP"
if [ "$verbose" != "" ] ; then
    echo "==== Checking signing $APP"
fi
codesign $verbose -d "$APP"

# update permissions and finish up
touch "$APP"

if [ "$verbose" != "" ] ; then
    echo  "==== Finished $APP"
fi
