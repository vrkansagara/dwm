/* See LICENSE file for copyright and license details. */

#include <X11/X.h>
#include <X11/XF86keysym.h> // /usr/include/X11/keysymdef.h
#include <X11/Xresource.h> // /usr/include/X11/Xresource.h
#include "layouts.c"
#include "debug.c"

/* Constants */
#define TERMINAL "st"
#define TERMINAL_CLASS "St"
#define EDITOR "vim"
#define MONITOR_DEFAULT 0
#define MONITOR_RIGHT 1
#define MONITOR_LEFT 2

#define TAG_GENERAL 1<<0
#define TAG_MESSAGING 1<<2
#define TAG_REMOTE 1<<3
#define TAG_UTILITY 1<<4
#define TAG_DEVELOPMENT 1<<7


// grep $USER /etc/passwd /** Is current user has default shell */
// chsh -s $(which zsh) /** Change default shell if not */
// grep zsh /etc/shells /** Is valid login shell *?
// TERMINAL_PATH will not be in effect once user has pre-defined shell.
#define TERMINAL_PATH "/bin/zsh"

// Brightness controll using (xdotool also useful)
// @TODO @FIXME brightness up/down require full home directory path
static const char *brightup[]       = { "/home/vrkansagara/.vim/bin/brightness", "up", "100", NULL };
static const char *brightdown[]     = { "/home/vrkansagara/.vim/bin/brightness", "down", "100", NULL };

// Volume specific settings (xdotool also useful)
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "+1%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "-1%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle",  NULL };


/* appearance */
static unsigned int borderpx  = 3;         //1 /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int showbar   = 1;        /* 0 means no bar */
static unsigned int topbar    = 1;        /* 0 means bottom bar */

static unsigned int gappih          = 20;       /* horizontal inner gap between windows */
static unsigned int gappiv          = 10;       /* vertical inner gap between windows */
static unsigned int gappoh          = 10;       /* horizontal outer gap between windows and screen edge */
static unsigned int gappov          = 30;       /* vertical outer gap between windows and screen edge */
static unsigned int smartgaps       = 0;        /* 1 means no outer gap when there is only one window */


static int swallowfloating          = 1;        /* 1 means swallow floating windows by default */

//static char *fonts[]          = { "monospace:size=10", "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
static const char *fonts[]          = {
    "Fira Code Medium:size=12:antialias=true:autohint=true",
//    "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true:color=true",
    "monospace:size=12",
};

//static const char dmenufont[]       = "monospace:size=10";
static char dmenufont[]       =  "Fira Code Medium:size=12:antialias=true:autohint=true";

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000"; // #005577
static char selbgcolor[]            = "#005577";

static char *colors[][3]      = {
    /*               fg         bg         border   */
//  [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
//  [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* initial layouts per tag ( Index of layouts[]  */
static const int initlayouts[] = { 0, 6, 0, 0, 6, 0, 2 ,2, 2 };

/* show/hide dmenu per tag ( Index of layouts[]  */
static const int initdmenu[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
 /* poxrop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     *  xprop | awk ' /^WM_CLASS/{sub(/.* =/, "instance:"); sub(/,/, "\nclass:"); print} /^WM_NAME/{sub(/.* =/, "title:"); print}'
     *  xprop -root -notype -f WM_NAME "8u"  |  sed -n -r 's/WM_NAME = \"(.*)\"/\1/p'
     */

//  /* class      instance    title       tags mask     isfloating   monitor */
//  { "Gimp",     NULL,       NULL,       0,            1,           -1 },
//  { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },

    /** class           instance,               title,              tags,               isfloating, isterminal ,noswallow              monitor */
    // Tag - 1 (General usse case)
    {   "Thunar",      "thunar",      NULL,       TAG_GENERAL,       0, 0,0,    MONITOR_DEFAULT},
    {   "Org.gnome.Nautilus",      "org.gnome.Nautilus",      NULL,       TAG_GENERAL,       0, 0,0,    MONITOR_DEFAULT},
    {   "thunderbird",      "Mail",             NULL,       TAG_GENERAL,       0, 0,0,    MONITOR_DEFAULT},

    // Tag - 2 (Terminal way on demand )
    {   TERMINAL_CLASS,     TERMINAL,                                        NULL,       1<<1,   0,  1,  0,  MONITOR_DEFAULT},

    // Tag - 3 (Socializing / Office on the way)
    {   "Skype",      "skype",                                  NULL,       TAG_MESSAGING,       0,  0,  0,    MONITOR_DEFAULT},
    {   "Microsoft Teams - Preview","microsoft teams - preview",NULL,       TAG_MESSAGING,       0,  0,  0,    MONITOR_DEFAULT},
    {   "ringcentral",      "ringcentral",                      NULL,       TAG_MESSAGING,       0,  0,  0,    MONITOR_DEFAULT},
    {   "Slack",      "slack",                                  NULL,       TAG_MESSAGING,       0,  0,  0,    MONITOR_LEFT},

    // Tag - 4 ( Remote things)
    {   "org.remmina.Remmina" ,"org.remmina.Remmina",       NULL,       TAG_REMOTE,   0,  0,  0,  MONITOR_LEFT},
    {   "Anydesk",      "anydesk",                          NULL,       TAG_REMOTE,   0,  0,  0,  MONITOR_LEFT},

    // Tag - 5 (Utilities)
    { "WorkComposer"    ,"workcomposer",        NULL,       TAG_UTILITY,       0,    0,  0,  MONITOR_LEFT},
    { "Hubstaff"    ,"Hubstaff",                NULL,       TAG_UTILITY,       0,    0,  0,  MONITOR_LEFT},
    { "openfortiGUI"    ,"openfortigui",        NULL,       TAG_UTILITY,       0,    0,  0,  MONITOR_LEFT},
    { "KeePassXC"    ,"keepassxc",              NULL,       TAG_UTILITY,       0,    0,  0,  MONITOR_LEFT},
    { "1Password"    ,"1password",              NULL,       TAG_UTILITY,       0,    0,  0,  MONITOR_LEFT},
    { "obs"    ,"obs",                          NULL,       TAG_UTILITY,       0,    0,  0,  MONITOR_LEFT},

    // Tag - 6 (Development : Light weight on memory)
    {   "vrkansagara-ide",               "vrkansagara-ide",     NULL,       1<<5,   0,  1,  0,  MONITOR_DEFAULT},
    {   "Geany",                    "geany",                    NULL,       1<<5,       0,           0,0,MONITOR_DEFAULT},
    {   "Postman",                  "postman",                NULL,       1<<5,       0,      0,0,MONITOR_DEFAULT},


    // Tag - 7 (Development : Medium/Heavy weight on memory)
    {   "jetbrains-clion",          "jetbrains-clion",          NULL,       1<<6,   0,  0,  0,  MONITOR_LEFT},
    {   "DBeaver",                  "DBeaver",                  NULL,       1<<6,       0,      0,0,MONITOR_LEFT},



    // Tag - 8 ( Heavy on memory )
    {   "jetbrains-phpstorm",       "jetbrains-phpstorm",    NULL,       TAG_DEVELOPMENT,  0,      0,0,MONITOR_DEFAULT},
    {   "code",                     "code",                  NULL,       TAG_DEVELOPMENT,  0,    0,0,MONITOR_DEFAULT},


    // Tag - 9 ( Things on WWW )
    { "firefox"                 ,"Navigator",               NULL,       1<<8,   0,  0,  0,  MONITOR_RIGHT},
    { "Google-chrome"           ,"google-chrome",           NULL,       1<<8,   0,  0,  0,  MONITOR_RIGHT},
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
//  /* symbol     arrange function */
//  { "[]=",      tile },    /* first entry is default */
//  { "><>",      NULL },    /* no layout function means floating behavior */
//  { "[M]",      monocle },

    /* symbol     arrange function */
    { "[]=",        tile },                     /* 0. Default: Master on left, slaves on right */
    { "[D]",        deck},                      /* 1. Master on left, slaves in monocle-like mode on right */

    { "[M]",        monocle },                  /* 2. All windows on top of eachother ( Full window ) */
    { "|M|",        centeredmaster },           /* 3. Master in middle, slaves on sides */
    { ">M>",        centeredfloatingmaster },   /* 4. Same but master floats */

    { "HHH",        grid },                     /* 5. Grid layout */
    { "TTT",        bstack},                    /* 6. Master on top, slaves on bottom */
    { "===",        bstackhoriz},               /* 7. Bstack horiz layout */

    { "><>",        NULL },                     /* 8. no layout function means floating behavior */
    { NULL,         NULL },                     /* ~ Failback */
};

/* Mask        | Value | Key */
/* ------------+-------+------------ */
/* LockMask    |     2 | Caps Lock */
/* ShiftMask   |     1 | Shift */
/* ControlMask |     4 | Ctrl */
/* Mod4Mask    |    64 | Windows */
/* Mod1Mask    |     8 | Alt */
/* Mod2Mask    |    16 | Num Lock */
/* Mod3Mask    |    32 | Scroll Lock */
/* Mod5Mask    |   128 | ??? */

/* key definitions */
// #define MODKEY Mod1Mask // Alt key for meta (Default as per DWM)
#define AltMask Mod1Mask  // Alt key for meta
#define MODKEY Mod4Mask // Window key for meta
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
/**
// Grave button = button immediately above the TAB on most keyboards.
{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
{ MOD, XK_x,     ACTION##stack, {.i = -1 } },
*/
#define STACKKEYS(MOD,ACTION) \
    { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
    { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
    { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ TERMINAL_PATH, "-c", cmd, NULL } }

// statuscmd with signals
#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };

static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
{ "font",               STRING,  &fonts },
{ "dmenufont",          STRING,  &dmenufont },

    { "color0",             STRING,     &normbgcolor },
    { "color0",             STRING,     &normbordercolor },
    { "color4",             STRING,     &normfgcolor },

    { "color4",             STRING,     &selbgcolor },
    { "color8",             STRING,     &selbordercolor },
    { "color0",             STRING,     &selfgcolor },

    { "borderpx",           INTEGER,    &borderpx },
    { "snap",               INTEGER,    &snap },

    { "showbar",            INTEGER,    &showbar },
    { "topbar",             INTEGER,    &topbar },

    { "nmaster",            INTEGER,    &nmaster },
    { "resizehints",        INTEGER,    &resizehints },
    { "mfact",              FLOAT,      &mfact },

    { "gappih",             INTEGER,    &gappih },
    { "gappiv",             INTEGER,    &gappiv },
    { "gappoh",             INTEGER,    &gappoh },
    { "gappov",             INTEGER,    &gappov },
    { "swallowfloating",    INTEGER,    &swallowfloating },
    { "smartgaps",          INTEGER,    &smartgaps },





};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    // Open dmenu
    { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
    // Open terminal
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    // Toggle tagbar
    { MODKEY,                       XK_b,      togglebar,      {0} },

    // Jump to another window ( stacker is taking ahead with jump so disable )
//  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
//  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

    // Increase/Decrease master
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },

    // Increase/Decrease master horizontal area
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },

    // Push window to master are
    { MODKEY,                       XK_Return, zoom,           {0} },
    // Jump to preview window, irrespective to tag number
    { MODKEY,                       XK_Tab,    view,           {0} },

    // Layout @START
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },        /* tile */
    { MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },        /* deck */

    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },        /* monocle */
    { MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[3]} },        /* centeredmaster */
    { MODKEY|ControlMask|ShiftMask, XK_m,      setlayout,      {.v = &layouts[4]} },        /* centeredfloatingmaster */

    { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} },        /* grid */
    { MODKEY|ShiftMask,             XK_g,      setlayout,      {.v = &layouts[6]} },        /* bstack (TTT) */
    { MODKEY|ShiftMask|ControlMask, XK_g,      setlayout,      {.v = &layouts[7]} },        /* bstackhoriz(===)*/

    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

    // Layout @END

    // Tag 1...2....9..0
    // View all windows
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    // View single window on all tags ( if you want to reset then reload dwm )
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },


    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)

    /* vrkansagara @START */

    // Patches(s) custom key(s)

    //Press MODKEY+s (default) to make a client 'sticky'. A sticky client is visible on all tags.
    { MODKEY,                       XK_s,      togglesticky,   {0} }, // Sticky window

    // Actually toggle fullscreen for a window, instead of toggling the status bar and the monocle layout.
    // This is similar to setting the client's tags to all 1's, but with the ability to easily return
    // it to its original tag by toggling it off by pressing MODKEY+s again.
    { MODKEY,                       XK_f,      togglefullscr,  {0} }, // Fullscreen window

    // vanitygaps
    { MODKEY|ShiftMask|ControlMask,                         XK_j,       incrgaps,       {.i = +3 } },
    { MODKEY|ShiftMask|ControlMask,                         XK_k,       incrgaps,       {.i = -3 } },
    { MODKEY|ShiftMask|ControlMask,                         XK_k,       incrigaps,       {.i = -3 } },
//  { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
//  { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
//  { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
//  { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
    { MODKEY|ShiftMask|ControlMask,                         XK_0,       defaultgaps,    {0}        },
    { MODKEY|ShiftMask|ControlMask|AltMask,                 XK_0,       togglegaps,     {0} },
// { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
// { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
// { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
// { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
// { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
// { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
// { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
// { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },


    // stacker ( focuse and change stack of slave
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)

    // GTK Clipboard manager
    { ControlMask|AltMask,                     XK_h,  spawn, SHCMD("diodon") },

    // Screen key for the keyboard
    { MODKEY|AltMask,                     XK_s,       spawn,          SHCMD("screenkey &") },
    { MODKEY|AltMask|ShiftMask,    XK_s,       spawn,          SHCMD("pkill -9 screenkey") },

    // dbeaver is also using alter+x which is sharp near by MODEKEY to make sure it not clicked.
    { MODKEY|ShiftMask,            XK_x,       spawn,          SHCMD("xkill") },
    { MODKEY,                      XK_Print,   spawn,          SHCMD("sxcs -o --hex | cut -f 2 | xclip -in -selection clipboard") },

    // Brightness related stuff
    { 0,        XF86XK_MonBrightnessUp,         spawn,          {.v = brightup } },
    { 0,        XF86XK_MonBrightnessDown,       spawn,          {.v = brightdown } },

    // Volume realated stuff
    { 0,        XF86XK_AudioLowerVolume,        spawn,          {.v = downvol } },
    { 0,        XF86XK_AudioMute,               spawn,          {.v = mutevol } },
    { 0,        XF86XK_AudioRaiseVolume,        spawn,          {.v = upvol   } },

    { 0,        XF86XK_Calculator,              spawn,          SHCMD(TERMINAL " -c calculator -n calculator -e bc -l") },
    { 0,        XK_Print,                       spawn,          SHCMD("flameshot gui --delay 0") },
    // Conflicts with selection of raw home and entire page up side.
//    { 0,        XK_Home,                        spawn,          SHCMD("nautilus $HOME") },

    /* start editor*/
    { MODKEY|ControlMask,           XK_Return, spawn,         SHCMD(TERMINAL " -c vrkansagara-ide -n vrkansagara-ide -e vim $HOME") },
    { MODKEY,                       XK_r,      spawn,         SHCMD(TERMINAL " -c htop -n htop -e htop -u $USER -d 60") },
    { MODKEY,                       XK_e,      spawn,         SHCMD(TERMINAL " -c ranger -n ranger -e ranger") },
    { MODKEY|ShiftMask,             XK_w,      spawn,         SHCMD(TERMINAL " -c nmtui -n nmtui -e sudo nmtui") },

    // To quit current window safely ( i.e. click on close window)
    { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    /* To quit dwm cleanly (It will hot reload all dwm config, see xinitrc for this) */
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    /* close all session of current $USER , use startx */
    { MODKEY|ShiftMask|ControlMask, XK_q,      spawn,          SHCMD(TERMINAL " pkill -u $USER -9")},

    /* vrkansagara @END */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        MODKEY,         Button2,        spawn,          {.v = termcmd } },

    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },

    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

