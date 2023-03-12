#!/bin/sh

case $BUTTON in
	1) notify-send "CPU usage" "$(ps axch -o cmd,%cpu --sort=-%cpu | head)" ;;
	3) st -e htop ;;
esac
