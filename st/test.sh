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
#  Note		  :- This is terminal test i.e. font color,size,bold,itali,underline
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

echo -e -n "\x1b[\x31 q" # Blinking block (default)
echo -e -n "\x1b[\x32 q" # Steady block
echo -e -n "\x1b[\x33 q" # Blinking underline
echo -e -n "\x1b[\x34 q" # Steady underline
echo -e -n "\x1b[\x35 q" # Blinking bar
echo -e -n "\x1b[\x36 q" # Steady bar
echo -e -n "\x1b[\x37 q" # Blinking st cursor
echo -e -n "\x1b[\x38 q" # Steady st cursor

echo -e "\e[1mbold\e[0m"
echo -e "\e[3mitalic\e[0m"
echo -e "\e[3m\e[1mbold italic\e[0m"
echo -e "\e[4munderline\e[0m"
echo -e "\e[9mstrikethrough\e[0m"
echo -e "\e[31mHello World\e[0m"
echo -e "\x1B[31mHello World\e[0m"


echo -e '\e[1mbold\e[22m'
echo -e '\e[2mdim\e[22m'
echo -e '\e[3mitalic\e[23m'
echo -e '\e[4munderline\e[24m'
echo -e '\e[4:1mthis is also underline (new in 0.52)\e[4:0m'
echo -e '\e[21mdouble underline (new in 0.52)\e[24m'
echo -e '\e[4:2mthis is also double underline (new in 0.52)\e[4:0m'
echo -e '\e[4:3mcurly underline (new in 0.52)\e[4:0m'
echo -e '\e[5mblink (new in 0.52)\e[25m'
echo -e '\e[7mreverse\e[27m'
echo -e '\e[8minvisible\e[28m <- invisible (but copy-pasteable)'
echo -e '\e[9mstrikethrough\e[29m'
echo -e '\e[53moverline (new in 0.52)\e[55m'

echo -e '\e[31mred\e[39m'
echo -e '\e[91mbright red\e[39m'
echo -e '\e[38:5:42m256-color, de jure standard (ITU-T T.416)\e[39m'
echo -e '\e[38;5;42m256-color, de facto standard (commonly used)\e[39m'
echo -e '\e[38:2::240:143:104mtruecolor, de jure standard (ITU-T T.416) (new in 0.52)\e[39m'
echo -e '\e[38:2:240:143:104mtruecolor, rarely used incorrect format (might be removed at some point)\e[39m'
echo -e '\e[38;2;240;143;104mtruecolor, de facto standard (commonly used)\e[39m'

echo -e '\e[46mcyan background\e[49m'
echo -e '\e[106mbright cyan background\e[49m'
echo -e '\e[48:5:42m256-color background, de jure standard (ITU-T T.416)\e[49m'
echo -e '\e[48;5;42m256-color background, de facto standard (commonly used)\e[49m'
echo -e '\e[48:2::240:143:104mtruecolor background, de jure standard (ITU-T T.416) (new in 0.52)\e[49m'
echo -e '\e[48:2:240:143:104mtruecolor background, rarely used incorrect format (might be removed at some point)\e[49m'
echo -e '\e[48;2;240;143;104mtruecolor background, de facto standard (commonly used)\e[49m'

echo -e '\e[21m\e[58:5:42m256-color underline (new in 0.52)\e[59m\e[24m'
echo -e '\e[21m\e[58;5;42m256-color underline (new in 0.52)\e[59m\e[24m'
echo -e '\e[4:3m\e[58:2::240:143:104mtruecolor underline (new in 0.52) (*)\e[59m\e[4:0m'
echo -e '\e[4:3m\e[58:2:240:143:104mtruecolor underline (new in 0.52) (might be removed at some point) (*)\e[59m\e[4:0m'
echo -e '\e[4:3m\e[58;2;240;143;104mtruecolor underline (new in 0.52) (*)\e[59m\e[4:0m'

# Testing ligatures
# 1 || 2
# 4 <= 5
# a =~ b
# 4 != 5
# Test::test
# Test->test()
# a++

for i in {0..255} ; do
    printf "\x1b[48;5;%sm%3d\e[0m " "$i" "$i"
    if (( i == 15 )) || (( i > 15 )) && (( (i-15) % 6 == 0 )); then
        printf "\n";
    fi
done