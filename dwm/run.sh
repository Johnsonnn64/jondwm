#!/bin/bash

Xephyr -br  -ac -reset -screen 3440x1440 :1 &
sleep 1s
export DISPLAY=:1
./dwm &
