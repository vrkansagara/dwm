#!/usr/bin/env bash
# set -eou pipefail
#set -e # This setting is telling the script to exit on a command error.
if [[ "$1" == "-v" ]]; then
  set -x # You refer to a noisy script.(Used to debugging)
fi


export DEBIAN_FRONTEND=noninteractive
CURRENT_DATE=$(date "+%Y%m%d%H%M%S")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
GREEN=$'\e[0;32m'
RED=$'\e[0;31m'
NC=$'\e[0m'

if [ "$(whoami)" != "root" ]; then
  SUDO=sudo
fi

apply_permission() {
  echo "$GREEN ${FUNCNAME[0]} to current directory is [$(pwd)]  $NC"
  # Give current user permission to work with source
  ${SUDO} chown $USER -Rf .
  ${SUDO} chgrp $USER -Rf .
}

# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#  Maintainer :- Vallabh Kansagara<vrkansagara@gmail.com> — @vrkansagara
#  Note       :- Submodule update script for the dwm project
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

echo "$GREEN Script running in this directory [$SCRIPT_DIR]  $NC"

# Lets make everything to local directory first
cd $SCRIPT_DIR
apply_permission

rm .gitmodules
touch .gitmodules
rm -rf .git/modules
rm -rf vendor/*
git submodule add -f https://github.com/torrinfail/dwmblocks.git vendor/dwmblocks
git submodule add -f https://github.com/N-R-K/sxcs vendor/sxcs
git submodule add -f git://git.suckless.org/dwm vendor/dwm
git submodule add -f git://git.suckless.org/dmenu vendor/dmenu
git submodule add -f git://git.suckless.org/st vendor/st
git submodule add -f git://git.suckless.org/slock vendor/slock
git submodule add -f git://git.suckless.org/scroll vendor/scroll

#git submodule foreach git reset --hard HEAD
#git submodule foreach git clean -fd
git submodule update --init --recursive --jobs 4  --remote --rebase
