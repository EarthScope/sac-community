#!/bin/sh

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

osascript  <<EOF
tell application "System Events" to set plist to get name of every process
tell application "Terminal"
    activate
    if (plist contains "Terminal") or ((count of every window) < 1) then
      tell application "System Events" to tell process "Terminal" to keystroke "n" using command down
    end if
    do script ("exec $APP_GUI") in front window
  end tell

EOF





