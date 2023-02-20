//Modify this file to change what commands output to your statusbar, and recompile using the make command.
//{"", "date '+%b %d (%a) %I:%M%p'",					5,		2},

static const Block blocks[] = {
/*Icon*/	/*Command*/		/*Update Interval(Secounds)*/	/*Update Signal*/
{"P:", "acpi -b | grep -P -o '[0-9]+(?=%)'",                        60,		0},
{"W:", "awk 'NR==3 {print $1 $3 00}' /proc/net/wireless",           60,		0},
{"Mem:", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g",	30,		1},
{"IST:", "TZ=':Asia/Kolkata' date +'%a %d %b[%r]'",                 1,      2},
{"CET:", "TZ=':Europe/Amsterdam' date +'%a %d %b[%r]'",             1,      3},
// {"", "date '+%b %d (%a) %I:%M%p'",5,4},
{"V", "~/.vim/bin/vtest",0,5},
//{"", "sb-battery",0,6},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
//static char delim[] = " | ";
static char delim[] = " | ";
static unsigned int delimLen = 5;
