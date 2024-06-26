#include "../dwm/debug.c"

//Modify this file to change what commands output to your statusbar, and recompile using the make command.
//{"", "date '+%b %d (%a) %I:%M%p'",                    5,      2},

//xprop -root -notype -f WM_NAME "8u"  |  sed -n -r 's/WM_NAME = \"(.*)\"/\1/p'
// if you see \001....\002 in output then understand that statuscmd is added to that block
static const Block blocks[] = {
    /*Icon*/    /*Command*/     /*Update Interval(Seconds)*/    /*Update Signal*/
   {"P: ", "acpi -b | grep -P -o '[0-9]+(?=%)'",                            60,     1},
    {"W: ", "awk 'NR==3 {print $1 \"/\" $3 00 \"%\"}' /proc/net/wireless",   60,       2},
   {"M: ", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g",       30,     3},
   {"", "sensors | grep -i rpm | awk '{ print \"Fan: \"$2\"/\"$10\" RPM\"}'",       1,     4},
//    {"", "TZ=':Asia/Kolkata' date +'%a %d %b [%r]'",                        1,      4},
//    {"", "TZ=':Europe/Amsterdam' date +'%a %d %b [%r]'",                    1,      5},
    // {"", "~/.vim/bin/statusbar/sb-disk",                                    5,      2},
    // {"", "~/.vim/bin/statusbar/sb-cpu",                                     5,      3},
    // {"", "~/.vim/bin/statusbar/sb-clock",                                   5,      4},
//     {"", "~/.vim/bin/statusbar/sb-battery",                                 5,      5},
    // {"", "date '+%b %d (%a) %I:%M%p'",5,4},
    // {"", "~/.vim/bin/statusbar/sb-internet",5,7},
    // {"", "~/.vim/bin/statusbar/sb-battery",5,8},
    // {"", "~/.vim/bin/statusbar/sb-memory",5,9},
    {"", "~/.vim/bin/dwmblocks",                                            1,      10},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
//static char delim[] = " | ";
static char delim[] = " | ";
static unsigned int delimLen = 5;

//wifi=`iwconfig 2>/dev/null | awk '/ESSID/' | cut -f 7- -d " " | cut -f 2- -d ":" | xargs` # get network name in quotes