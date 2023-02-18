rm .gitmodules
touch .gitmodules
rm -rf .git/modules
rm -rf vendor/*
git submodule add -f https://github.com/torrinfail/dwmblocks.git
git submodule add -f git://git.suckless.org/dwm vendor/dwm
git submodule add -f git://git.suckless.org/dmenu vendor/dmenu
git submodule add -f git://git.suckless.org/st vendor/st
git submodule add -f git://git.suckless.org/slock vendor/slock
git submodule add -f git://git.suckless.org/scroll vendor/scroll
git submodule add -f https://github.com/N-R-K/sxcs vendor/sxcs

git submodule foreach git reset --hard HEAD
git submodule foreach git clean -fd
git submodule update --init --recursive --jobs 4  --remote --rebase
