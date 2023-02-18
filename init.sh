ps -ef | grep "dwmblocks" | grep -v grep | awk "{print \$2}" | xargs --no-run-if-empty sudo kill -9jj
/usr/local/bin/dwmblocks&%

# Required sxcs
sudo apt install libxcursor-dev