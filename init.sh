ps -ef | grep "dwmblocks" | grep -v grep | awk "{print \$2}" | xargs --no-run-if-empty sudo kill -9
/usr/local/bin/dwmblocks&

# Required sxcs
sudo apt install libxcursor-dev

#sudo apt install  fonts-noto-color-emoji
#mkdir -p ~/.fonts/NotoEmoji
curl -L 'https://raw.githubusercontent.com/googlefonts/noto-emoji/main/fonts/NotoColorEmoji.ttf' -o ~/.fonts/NotoEmoji/NotoColorEmoji.ttf
#fc-cache -fv

sudo apt remove --purge fonts-noto-color-emoji unifont

 
sudo fc-cache -fv
fc-list | grep -i emoji

echo "🩷💀🫱"
