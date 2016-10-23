#!/bin/sh

while true;
do
	if test -n "`ps -e | grep plugin-containe`";then
		gnome-screensaver-command --deactivate >/dev/null && xset -dpms
	else
		xset dpms
	fi
	sleep 30
done
