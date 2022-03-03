#include <X11/Xlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
//#define CMDLENGTH		50
//#define CMDLENGTH		80
#define CMDLENGTH 100

#include "plugin/mpdstatus.c"
#include "plugin/nettraf.c"
#include "plugin/battery.c"

typedef struct {
  char *icon;
  char *command;
  unsigned int interval;
  unsigned int signal;
  char *(*func)();
} Block;
void sighandler(int num);
void getcmds(int time);
#ifndef __OpenBSD__
void getsigcmds(int signal);
void setupsignals();
void sighandler(int signum);
#endif
int getstatus(char *str, char *last);
void setroot();
void statusloop();
void termhandler(int signum);

#include "blocks.h"

static Display *dpy;
static int screen;
static Window root;
static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
/* static char statusstr[2][256]; */
static char statusstr[2][512];
static int statusContinue = 1;
static void (*writestatus)() = setroot;

// opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output) {
  //  if (*block->icon != 'c') {
  int i = strlen(block->icon);
  strcpy(output, block->icon);
  if (block->command) {
    char *cmd = block->command;
    FILE *cmdf = popen(cmd, "r");
    if (!cmdf)
      return;
    char c;
    fgets(output + i, CMDLENGTH - i, cmdf);
    pclose(cmdf);
  } else {
 //   strcpy(output + i, block->func());
//    memcpy(output + i, block->func(), CMDLENGTH -1 - i);
    memcpy(output + i, block->func(), CMDLENGTH - i);
  }
  i = strlen(output);
  if (delim != '\0' && --i)
    output[i++] = delim;
  output[i++] = '\0';
  // char * (*funcP)();
  // funcP = getmpdstat;
  // strcpy(output, (*funcP)());
}

void getcmds(int time) {
  const Block *current;
  for (int i = 0; i < LENGTH(blocks); i++) {
    current = blocks + i;
    if ((current->interval != 0 && time % current->interval == 0) || time == -1)
      getcmd(current, statusbar[i]);
  }
}

#ifndef __OpenBSD__
void getsigcmds(int signal) {
  const Block *current;
  for (int i = 0; i < LENGTH(blocks); i++) {
    current = blocks + i;
    if (current->signal == signal)
      getcmd(current, statusbar[i]);
  }
}

void setupsignals() {
  for (int i = 0; i < LENGTH(blocks); i++) {
    if (blocks[i].signal > 0)
      signal(SIGRTMIN + blocks[i].signal, sighandler);
  }
}
#endif

int getstatus(char *str, char *last) {
  strcpy(last, str);
  str[0] = '\0';
  for (int i = 0; i < LENGTH(blocks); i++)
    strcat(str, statusbar[i]);
  str[strlen(str) - 1] = '\0';
  return strcmp(str, last); // 0 if they are the same
}

void setroot() {
  if (!getstatus(statusstr[0],
                 statusstr[1])) // Only set root if text has changed.
    return;
  Display *d = XOpenDisplay(NULL);
  if (d) {
    dpy = d;
  }
  screen = DefaultScreen(dpy);
  root = RootWindow(dpy, screen);
  XStoreName(dpy, root, statusstr[0]);
  XCloseDisplay(dpy);
}

void pstdout() {
  if (!getstatus(statusstr[0],
                 statusstr[1])) // Only write out if text has changed.
    return;
  printf("%s\n", statusstr[0]);
  fflush(stdout);
}

void statusloop() {
#ifndef __OpenBSD__
  setupsignals();
#endif
  int i = 0;
  getcmds(-1);
  while (statusContinue) {
    getcmds(i);
    writestatus();
    sleep(1.0);
    //usleep(1000000);
    i++;
  }
}

#ifndef __OpenBSD__
void sighandler(int signum) {
  getsigcmds(signum - SIGRTMIN);
  writestatus();
}
#endif

void termhandler(int signum) {
  statusContinue = 0;
  exit(0);
}

int main(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp("-d", argv[i]))
      delim = argv[++i][0];
    else if (!strcmp("-p", argv[i]))
      writestatus = pstdout;
  }
  //system("(pidof -x mpdidle > /dev/null && notify-send 'mpdidle already start') || (notify-send 'mpdidle start' && mpdidle & )");
  signal(SIGTERM, termhandler);
  signal(SIGINT, termhandler);
  statusloop();
}
