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
SXCS_DIR="$SCRIPT_DIR/vendor/sxcs"

if [ "$(whoami)" != "root" ]; then
  SUDO=sudo
fi

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

ini_required(){
  # Required sxcs
  ${SUDO} apt install libxft-dev libxinerama-dev xcb libxcb-xkb-dev libx11-xcb-dev libxcb-res0-dev libxrandr-dev
sudo  apt-get install --yes --no-install-recommends \
    xcb libxcb-xkb-dev \
    x11-xkb-utils libx11-xcb-dev \
    libxkbcommon-x11-dev libxcb-res0-dev suckless-tools
    
  ${SUDO} apt install make curl zsh feh libxcursor-dev xautolock screenkey libimlib2-dev flameshot cpulimit compton inxi

  #Google noto font is not supporting so remove it so dwm,st or dwmblock should not crash
  ${SUDO} apt remove --purge fonts-noto-color-emoji unifont

  #${SUDO apt install  fonts-noto-color-emoji
  #mkdir -p ~/.fonts/NotoEmoji
  #curl -L 'https://raw.githubusercontent.com/googlefonts/noto-emoji/main/fonts/NotoColorEmoji.ttf' -o ~/.fonts/NotoEmoji/NotoColorEmoji.ttf
  #fc-cache -fv
#  sudo fc-cache -fv
#  fc-list | grep -i emoji
#  echo "🩷💀🫱"
}

GREEN=$'\e[0;32m'
RED=$'\e[0;31m'
NC=$'\e[0m'

# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#  Maintainer :- Vallabh Kansagara<vrkansagara@gmail.com> — @vrkansagara
#  Note       :- DWM Window manager initial script
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

${SUDO} sudo apt-get install --yes -q --no-install-recommends \
lightdm xorg openbox
${SUDO} dpkg-reconfigure lightdm

${SUDO} sudo apt-get install --yes -q --no-install-recommends \
  patch build-essential \
  feh cpulimit cputool screenkey htop zsh tree vlc git nmap elinks vim gimp \
  arandr suckless-tools xautolock ranger\
  alsa-utils mesa-utils pulseaudio pavucontrol \
  notification-daemon notify-osd  libnotify-dev libnotify-bin \
  network-manager iputils-ping net-tools lsof whois \
  hardinfo inxi lshw hddtemp net-tools ipmitool nvme-cli \
  freeipmi-tools ipvsadm lvm2 mdadm lm-sensors smartmontools \

# libpoppler-cpp-dev pkg-config python3-dev \
#  libx11-dev x11proto-core-dev libxft-dev libharfbuzz-dev libxinerama-dev libxinerama1 libio-socket-ssl-perl libcpanel-json-xs-perl libjson-xs-perl libxml-dumper-perl xdotool \
#  conky keepassxc \
#  thunar  xhk wmctrl\
#  linux-headers-$(uname -r) linux-image-$(uname -r) lsb-release \
#  linux-image-$(uname -r | sed 's,[^-]*-[^-]*-,,') linux-headers-$(uname -r | sed 's,[^-]*-[^-]*-,,') at-spi2-core firmware-linux-nonfree \
#  broadcom-sta-dkms at-spi2-core

# pulseaudio --start --log-target=syslogk

# scrot = screen capture tool
# compton = dwm/st tranparency with x11
# xdotool = programmetly call keybindings
# inxi = forensic tool for hardware information
# xautolock = monitor binary for x time

# slock = suckless login manger
# suckless-tools = suckless built in library

# Check if compositor is running or not
# inxi -Gxx | grep compositor

# xautolock -time 1 -locker slock
# ${SUDO} cp -R hooks .git/

echo "$GREEN Script running in this directory [$SCRIPT_DIR]  $NC"

apply_permission
ini_required

# DWM Specific
cd $DWM_DIR
apply_permission
apply_patche
${SUDO} make clean
${SUDO} make
${SUDO} make uninstall
${SUDO} make install

# sxcs Specific
cd $SXCS_DIR
apply_permission
${SUDO} make clean
${SUDO} make
${SUDO} make uninstall
${SUDO} make install


# DWMBlock Specific
cd $DWMBLOCKS_DIR
apply_permission
apply_patche
${SUDO} make clean
${SUDO} make
${SUDO} make uninstall
${SUDO} make install
#Lets kill all process which is executed for the dwmblocks
ps -ef | grep "dwmblocks" | grep -v grep | awk "{print \$2}" | xargs --no-run-if-empty sudo kill -9
# reset statusbar
xsetroot -name ""
/usr/local/bin/dwmblocks  2>&1 >> $HOME/tmp/dwmblocks.log &

${SUDO} rm -rf /usr/share/xsessions/vallabh.desktop
${SUDO} ln -P $DWM_DIR/dwm.desktop /usr/share/xsessions/vallabh.desktop
${SUDO} rm -rf $HOME/.xinitrc $HOME/.xprofile
${SUDO} ln -P $DWM_DIR/x11/xinitrc $HOME/.xinitrc
${SUDO} ln -P $DWM_DIR/x11/xprofile $HOME/.xprofile
${SUDO} chmod 744 $HOME/.xinitrc
${SUDO} chmod u+s /usr/bin/xinit


# Copy conky configuration to home folder
# ${SUDO} rm -rf $HOME/.config/conky
# ${SUDO} cp -R $(pwd)/conky $HOME/.config

${SUDO} rm -rf $HOME/.config/ranger
${SUDO} ln -s $HOME/git/vrkansagara/dwm/ranger $HOME/.config/

${SUDO} chown $USER -Rf $HOME/.config
${SUDO} chgrp $USER -Rf $HOME/.config

${SUDO} chsh -s $(which zsh) $USER
${SUDO} chmod u+s $HOME/.vim/bin/*  $SCRIPT_DIR/bin/*

# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=747465
# echo '[D-BUS Service]
# Name=org.freedesktop.Notifications
# Exec=/usr/lib/notification-daemon/notification-daemon'| ${SUDO} tee /usr/share/dbus-1/services/org.gnome.Notifications.service > /dev/null

# Command line fuzzy finder called fzf
if [ ! -d "$HOME/.fzf" ]; then
    cd $HOME
    git clone https://github.com/junegunn/fzf.git --depth=1 -b master .fzf
    cd .fzf
    ${SUDO} git stash
    git reset --hard HEAD
    git clean -fd
fi


echo "$GREEN Your simple window manager is configured and ready to use.........[DONE]. $NC"
exit 0

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

#sudo apt-get install libx11-dev ................. for X11/Xlib.h
#sudo apt-get install mesa-common-dev........ for GL/glx.h
#sudo apt-get install libglu1-mesa-dev ..... for GL/glu.h
#sudo apt-get install libxrandr-dev ........... for X11/extensions/Xrandr.h
#sudo apt-get install libxi-dev ................... for X11/extensions/XInput.h