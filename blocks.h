#ifndef BLOCK_H
#define BLOCK_H

/* stddef is for NULL define */
#include <stddef.h>

char *smprintf(char *fmt, ...);

char *getmpdstat();
char *get_netusage();
char *cpu_usage();
char *getbattery();

typedef struct {
  char *icon;
  char *command;
  unsigned int interval;
  unsigned int signal;
  char *(*func)();
} Block;

//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		                                        /*Update Interval*/	/*Update Signal*/

    {"", NULL,				                                             0,		            12, getmpdstat},

    {"", NULL,				                                             1,		             0, get_netusage},

	{"", "/home/jiang/.dwmbar/statusbar/BTBattery",                     30,		             0, NULL},

	{"", "/home/jiang/.dwmbar/statusbar/volume",                         0,		            10, NULL},

	{"", NULL,			                                                 0,		            11, NULL},

    {"", NULL,				                                             1,		             0, cpu_usage},

	{"", "/home/jiang/.dwmbar/statusbar/memory",                         1,		             0, NULL},

    {" ", "sensors | awk '/^temp1:/{print $2}'|sed -n '3p'",		     3,		             0, NULL},

    {"", NULL,				                                            60,		             0, getbattery},

	{"", "/home/jiang/.dwmbar/statusbar/clock",						    60,		             0, NULL},

};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim = ' ';

#endif
