/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nogroup";

static const char *colorname[NUMCOLS] = {
	[INIT] =   "black",     /* after initialization */
	[INPUT] =  "#002B36", //"#005577",   /* during input */
	[FAILED] = "#AF6457", //"#CC3333",   /* wrong password */
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;

/* Background image path, should be available to the user above */
//static const char* background_image = "/home/vallabh/git/vrkansagara/dwm/dwm/images/Unix.png";
//static const char* background_image = "";

/* number of failed password attempts until failcommand is executed.
   Set to 0 to disable */
static const int failcount = 1;

/* command to be executed after [failcount] failed password attempts */
//static const char *failcommand = "echo \"Login failure att $(date '+%Y%m%d%H%M%S')\" >> /tmp/slock.log";
static const char *failcommand = "sudo shutdown now";

/* default message */
static const char * message = "VRKANSAGARA: Time and Tide Wait For none !";

/* text color */
static const char * text_color = "#002B36";

/* text size (must be a valid size) */
static const char * text_size = "6x10";