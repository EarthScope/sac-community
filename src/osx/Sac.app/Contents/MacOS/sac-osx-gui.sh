#!/bin/sh

RUN_IN_TERMINAL=1

export APP_PROGRAM=$0

function get_bundle_path() {
    TMP="${APP_PROGRAM}"
    TMP=`dirname "$TMP"`
    TMP=`dirname "$TMP"`
    echo "$TMP"
}
export APP_BUNDLE=`get_bundle_path`
export APP_RESOURCE="${APP_BUNDLE}/Resources"
export APP_GUI="${APP_BUNDLE}/MacOS/sac-osx-gui"

export SACAUX="${APP_RESOURCE}/aux"
export SACGRAPHICSDEVICE="MacOSX"

if [ $RUN_IN_TERMINAL -eq 1 ] ; then
osascript  <<EOF 
tell Application "Terminal"
   set currentTab to do script "export SACAUX=$SACAUX"
   do script "export SACGRAPHICSDEVICE=$SACGRAPHICSDEVICE" in currentTab
   do script "$APP_GUI && exit" in currentTab
end tell
EOF
else 
    $APP_GUI
fi




