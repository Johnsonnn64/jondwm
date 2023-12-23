#!/bin/bash

Xephyr -br  -ac -reset -screen 1920x1080 :1 &
sleep 1s
export DISPLAY=:1
./dwm &
feh --bg-fill --no-fehbg /home/john/personal/pictures/wallpapers/misc/windows-error.jpg
