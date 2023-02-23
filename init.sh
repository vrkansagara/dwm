#!/usr/bin/env bash
# set -eou pipefail
#set -e # This setting is telling the script to exit on a command error.
if [[ "$1" == "-v" ]]; then
  set -x # You refer to a noisy script.(Used to debugging)
fi

export DEBIAN_FRONTEND=noninteractive
CURRENT_DATE=$(date "+%Y%m%d%H%M%S")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

DMENU_DIR="$SCRIPT_DIR/dmenu"
DWM_DIR="$SCRIPT_DIR/dwm"
DWMBLOCKS_DIR="$SCRIPT_DIR/dwmblocks"
SLOCK_DIR="$SCRIPT_DIR/slock"
ST_DIR="$SCRIPT_DIR/st"
SCROLL_DIR="$SCRIPT_DIR/vendor/scroll"

apply_permission() {
  echo "$RED Current directory is [$(pwd)]  $NC"
  # Give current user permission to work with source
  ${SUDO} chown $USER -Rf .
  ${SUDO} chgrp $USER -Rf .
}

apply_patche(){
  FILES="$(pwd)/patches/*.diff"
  for f in $FILES; do
      if [ -f "$f" ]; then
          dos2unix $f
          echo "$RED Applying path for the [ $f ] $NC"
          patch --merge=diff3 -i $f
          retCode=$$?; \
          [[ $$retCode -gt 1 ]] && exit $$retCode; \
          sleep 1
      fi
  done
}

apply_git_clean(){
echo "$RED Git clean apply to [$(pwd)]  $NC"
  ${SUDO} git reset --hard HEAD
  ${SUDO} git clean -fd
  apply_permission
}

if [ "$(whoami)" != "root" ]; then
  SUDO=sudo
fi

# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#  Maintainer :- Vallabh Kansagara<vrkansagara@gmail.com> — @vrkansagara
#  Note       :- DWM Window manager initial script
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


GREEN=$'\e[0;32m'
RED=$'\e[0;31m'
NC=$'\e[0m'
echo "$GREEN Script running in this directory [$SCRIPT_DIR]  $NC"


# DWM Specific
cd $DWM_DIR
apply_patche
${SUDO} make clean
${SUDO} make
${SUDO} make uninstall
${SUDO} make install

exit
ps -ef | grep "dwmblocks" | grep -v grep | awk "{print \$2}" | xargs --no-run-if-empty sudo kill -9
/usr/local/bin/dwmblocks&

# #base03    #002b36  background
# #base02    #073642  background heightlight
# #base01    #586e75  Comments/ secondary content
#base00    #657b83
#base0     #839496
#base1     #93a1a1
#base2     #eee8d5
#base3     #fdf6e3
#yellow    #b58900
#orange    #cb4b16
#red       #dc322f
#magenta   #d33682
#violet    #6c71c4
#blue      #268bd2
#cyan      #2aa198
#green     #859900

# Required sxcs
sudo apt install libxcursor-dev xautolock screenkey libimlib2-dev flameshot cpulimit

#sudo apt install  fonts-noto-color-emoji
#mkdir -p ~/.fonts/NotoEmoji
#curl -L 'https://raw.githubusercontent.com/googlefonts/noto-emoji/main/fonts/NotoColorEmoji.ttf' -o ~/.fonts/NotoEmoji/NotoColorEmoji.ttf
#fc-cache -fv

sudo apt remove --purge fonts-noto-color-emoji unifont

sudo fc-cache -fv
fc-list | grep -i emoji
echo "🩷💀🫱"
