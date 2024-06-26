#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include <errno.h>
#include<sys/wait.h>
#ifndef NO_X
#include<X11/Xlib.h>
#endif
#ifdef __OpenBSD__
#define SIGPLUS			SIGUSR1+1
#define SIGMINUS		SIGUSR1-1
#else
#define SIGPLUS			SIGRTMIN
#define SIGMINUS		SIGRTMIN
#endif
#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))
#define CMDLENGTH		50
#define MIN( a, b ) ( ( a < b) ? a : b )
#define STATUSLENGTH (LENGTH(blocks) * CMDLENGTH + 1)

typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;
#ifndef __OpenBSD__
void dummysighandler(int num);
#endif
void replace(char *str, char old, char new);
void remove_all(char *str, char to_remove);
void getcmds(int time);
void getsigcmds(unsigned int signal);
void setupsignals();
void sighandler(int signum, siginfo_t *si, void *ucontext);
int getstatus(char *str, char *last);
void statusloop();
void statusloop_improved();
void getcmd_improved(const Block *block, char *output);
void termhandler();
void chldhandler();
void pstdout();
#ifndef NO_X
void setroot();
static void (*writestatus) () = setroot;
static int setupX();
static Display *dpy;
static int screen;
static Window root;
#else
static void (*writestatus) () = pstdout;
#endif


#include "blocks.h"

static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][STATUSLENGTH];
static int statusContinue = 1;
static int returnStatus = 0;

void replace(char *str, char old, char new)
{
	for(char * c = str; *c; c++)
		if(*c == old)
			*c = new;
}

// the previous function looked nice but unfortunately it didn't work if to_remove was in any position other than the last character
// theres probably still a better way of doing this
void remove_all(char *str, char to_remove) {
	char *read = str;
	char *write = str;
	while (*read) {
		if (*read != to_remove) {
			*write++ = *read;
		}
		++read;
	}
	*write = '\0';
}

int gcd(int a, int b)
{
	int temp;
	while (b > 0){
		temp = a % b;

		a = b;
		b = temp;
	}
	return a;
}

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output)
{
	if (block->signal)
		*output++ = block->signal;
	strcpy(output, block->icon);
	FILE *cmdf = popen(block->command, "r");
	if (!cmdf)
		return;
	int i = strlen(block->icon);
	fgets(output+i, CMDLENGTH-i-delimLen, cmdf);
	remove_all(output, '\n');
	i = strlen(output);
	if (i == 0) {
		//return if block and command output are both empty
		pclose(cmdf);
		return;
	}
	//only chop off newline if one is present at the end
	i = output[i-1] == '\n' ? i-1 : i;
	if (delim[0] != '\0') {
		strncpy(output+i, delim, delimLen); 
	}
	else
		output[i++] = '\0';
	pclose(cmdf);
}

void getcmd_improved(const Block *block, char *output)
{
	if (block->signal)
	{
		output[0] = block->signal;
		output++;
	}
	char *cmd = block->command;
	FILE *cmdf = popen(cmd,"r");
	if (!cmdf){
        //printf("failed to run: %s, %d\n", block->command, errno);
        fprint_dbg(IS_DEBUG_ON,"failed to run: %s, %d\n", block->command, errno);
		return;
    }
    char tmpstr[CMDLENGTH] = "";
    // TODO decide whether its better to use the last value till next time or just keep trying while the error was the interrupt
    // this keeps trying to read if it got nothing and the error was an interrupt
    //  could also just read to a separate buffer and not move the data over if interrupted
    //  this way will take longer trying to complete 1 thing but will get it done
    //  the other way will move on to keep going with everything and the part that failed to read will be wrong till its updated again
    // either way you have to save the data to a temp buffer because when it fails it writes nothing and then then it gets displayed before this finishes
	char * s;
    int e;
    do {
        errno = 0;
        s = fgets(tmpstr, CMDLENGTH-(strlen(delim)+1), cmdf);
        e = errno;
    } while (!s && e == EINTR);
	pclose(cmdf);
	int i = strlen(block->icon);
	strcpy(output, block->icon);
    strcpy(output+i, tmpstr);
	remove_all(output, '\n');
	i = strlen(output);
    if ((i > 0 && block != &blocks[LENGTH(blocks) - 1])){
        strcat(output, delim);
    }
    i+=strlen(delim);
	output[i++] = '\0';
}

void getcmds(int time)
{
// This function first creates a pointer to a Block structure and afterwards loops over all blocks.
// On each iteration a condition is to be met, IF the current blocks refresh/re-execute interval is not equal to 0
// AND the time argument passed to the getcmds( int time) function can be divided by that blocks interval without
// any remainder [% mod operation] OR the time argument is just equal to -1,
// THEN run the getcmd function and pass it the current block and the i-th object/pointer in the statusbar array.

	const Block* current;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if ((current->interval != 0 && time % current->interval == 0) || time == -1){
		    getcmd(current,statusbar[i]);
//		    getcmd_improved(current,statusbar[i]);
        }
	}
}

void getsigcmds(unsigned int signal)
{
	const Block *current;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if (current->signal == signal){
		    getcmd(current,statusbar[i]);
//		    getcmd_improved(current,statusbar[i]);
        }
	}
}

void setupsignals()
{
	struct sigaction sa = { .sa_sigaction = sighandler, .sa_flags = SA_SIGINFO };
#ifndef __OpenBSD__
	    /* initialize all real time signals with dummy handler */
    for (int i = SIGRTMIN; i <= SIGRTMAX; i++) {
        signal(i, dummysighandler);
		sigaddset(&sa.sa_mask, i);
	}
#endif

	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		if (blocks[i].signal > 0){
		    sigaction(SIGMINUS+blocks[i].signal, &sa, NULL);
		}
	}

}

int getstatus(char *str, char *last)
{
	strcpy(last, str);
	str[0] = '\0';
	for (unsigned int i = 0; i < LENGTH(blocks); i++)
		strcat(str, statusbar[i]);
	str[strlen(str)-strlen(delim)] = '\0';
	return strcmp(str, last);//0 if they are the same
}

#ifndef NO_X
void setroot()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only set root if text has changed.
		return;
	XStoreName(dpy, root, statusstr[0]);
	XFlush(dpy);
}

int setupX()
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		fprintf(stderr, "dwmblocks: Failed to open display\n");
		return 0;
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	return 1;
}
#endif

void pstdout()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;
	printf("%s\n",statusstr[0]);
	fflush(stdout);
}

// infinite loop where the program gets the commands (simply said executes them and stores the output in an array)
void statusloop()
{
	setupsignals();
	int i = 0;
	getcmds(-1);
	while (1) {
		getcmds(i++);
		writestatus();
		if (!statusContinue)
			break;
		sleep(1.0);
	}
}

void statusloop_improved()
{
#ifndef __OpenBSD__
	setupsignals();
#endif
    // first figure out the default wait interval by finding the
    // greatest common denominator of the intervals
    unsigned int interval = -1;
    for(int i = 0; i < LENGTH(blocks); i++){
        if(blocks[i].interval){
            interval = gcd(blocks[i].interval, interval);
        }
    }
	unsigned int i = 0;
    int interrupted = 0;
    const struct timespec sleeptime = {interval, 0};
    struct timespec tosleep = sleeptime;
	getcmds(-1);
	while(statusContinue)
	{
        // sleep for tosleep (should be a sleeptime of interval seconds) and put what was left if interrupted back into tosleep
        interrupted = nanosleep(&tosleep, &tosleep);
        // if interrupted then just go sleep again for the remaining time
        if(interrupted == -1){
            continue;
        }
        // if not interrupted then do the calling and writing
        getcmds(i);
        writestatus();
        // then increment since its actually been a second (plus the time it took the commands to run)
        i += interval;
        // set the time to sleep back to the sleeptime of 1s
        tosleep = sleeptime;
	}
}

#ifndef __OpenBSD__
/* this signal handler should do nothing */
void dummysighandler(int signum)
{
    fprint_dbg(IS_DEBUG_ON,"DWMBLOCKS dummysighandler = [%d] ",signum);
    return;
}
#endif

// The sighandler function first gets the signal commands for a specific signal and then writes the commands output to the status bar by using
void sighandler(int signum, siginfo_t *si, void *ucontext)
{
    /* if button is zero, the signal is not from a button press */
    int button = si->si_value.sival_int; /* if button is zero, the signal is not from a button press */
    int signal = signum - SIGRTMIN;

	if (button) {
		pid_t parent = getpid();
		if (fork() == 0) {
#ifndef NO_X
			if (dpy)
				close(ConnectionNumber(dpy));
#endif
			int i;
			for (i = 0; i < LENGTH(blocks) && blocks[i].signal != signal; i++);

			char shcmd[1024];
			sprintf(shcmd, "%s; kill -%d %d", blocks[i].command, SIGRTMIN+blocks[i].signal, parent);
//			sprintf(shcmd, "%s; kill -%d %d", blocks[i].command, SIGRTMIN+blocks[i].signal+34, parent);
			fprint_dbg(IS_DEBUG_ON,"shcmd = [%s]",shcmd);
			char *cmd[] = { "/bin/sh", "-c", shcmd, NULL };
			char button[2] = { '0' + si->si_value.sival_int, '\0' };
			setenv("BLOCK_BUTTON", button, 1);
			setsid();
			execvp(cmd[0], cmd);
			perror(cmd[0]);
			exit(EXIT_SUCCESS);
		}
	} else {
	    fprint_dbg(IS_DEBUG_ON,"else.... = [%d]",signum-SIGPLUS);
        // getsigcmds() is analogous to getcmds() get commands function.
        getsigcmds(signum-SIGPLUS);
        writestatus();
	}
}

void termhandler()
{
//Additional description for termhandler: termhandler() sets the continue execution flag for the status loop function. Once set to 0 the status loop function will exit and the program will terminate.
	statusContinue = 0;
	exit(0);
}

void chldhandler()
{
	while (0 < waitpid(-1, NULL, WNOHANG));
}

int main(int argc, char** argv)
{
    fprint_dbg(IS_DEBUG_ON,"DWMBLOCKS start");

	for (int i = 0; i < argc; i++) {//Handle command line arguments
		if (!strcmp("-d",argv[i]))
			strncpy(delim, argv[++i], delimLen);
		else if (!strcmp("-p",argv[i]))
			writestatus = pstdout;
	}

#ifndef NO_X
	if (!setupX())
		return 1;
#endif

	delimLen = MIN(delimLen, strlen(delim));
	delim[delimLen++] = '\0';

//	SIGTERM = (signal 15) is a request to the program to terminate.
//	SIGKILL = (signal 9) is a directive to kill the process immediately. This signal cannot be caught or ignored.
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);

//	Sends a SIGCHLD signal to the parent process to indicate that the child process has ended
//	signal(SIGCHLD, chldhandler);

	statusloop();
//	statusloop_improved();

#ifndef NO_X
	XCloseDisplay(dpy);
#endif


    fprint_dbg(IS_DEBUG_ON,"DWMBLOCKS end");
	return returnStatus;
}
