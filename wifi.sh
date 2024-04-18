sudo apt install rfkill  network-manager 
sudo rfkill unblock all
sudo nmcli radio
sudo ip link set wlp2s0 down
sudo ip link set wlp2s0 up
