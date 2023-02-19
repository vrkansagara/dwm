#!/usr/bin/env bash
# set -e # This setting is telling the script to exit on a command error.
# set -x # You refer to a noisy script.(Used to debugging)

echo ""
export DEBIAN_FRONTEND=noninteractive
CURRENT_DATE=$(date "+%Y%m%d%H%M%S")
SCRIPT=$(readlink -f "")
SCRIPTDIR=$(dirname "$SCRIPT")

if [ "$(whoami)" != "root" ]; then
SUDO=sudo
fi

# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#  Maintainer :- Vallabh Kansagara<vrkansagara@gmail.com> — @vrkansagara
#  Note		  :- test images....
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

# img=(`find ${SCRIPTDIR} -name '*' -exec file {} \; | grep -o -P '^.+: \w+ image' | cut -d':' -f1`)
# feh --bg-scale "${img[$RANDOM % ${#img[@]} ]}"
while true; do
  find ${SCRIPTDIR} -type f -iname "*.jpg" | shuf -n 1 | xargs feh --bg-fill
  # sleep 30m
  sleep  1
done

