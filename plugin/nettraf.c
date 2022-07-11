#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../utils.h"

static unsigned long long int rec, sent;

int
parse_netdev(unsigned long long int *receivedabs, unsigned long long int *sentabs)
{
	char buf[255];
	char *datastart;
	static int bufsize;
	int rval;
	FILE *devfd;
	unsigned long long int receivedacc, sentacc;

	bufsize = 255;
	devfd = fopen("/proc/net/dev", "r");
	rval = 1;

	// Ignore the first two lines of the file
	fgets(buf, bufsize, devfd);
	fgets(buf, bufsize, devfd);

	while (fgets(buf, bufsize, devfd)) {
	    if ((datastart = strstr(buf, "lo:")) == NULL) {
		datastart = strstr(buf, ":");

		// With thanks to the conky project at http://conky.sourceforge.net/
		sscanf(datastart + 1, "%llu  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %llu",\
		       &receivedacc, &sentacc);
		*receivedabs += receivedacc;
		*sentabs += sentacc;
		rval = 0;
	    }
	}

	fclose(devfd);
	return rval;
}

void
calculate_speed(char *speedstr, unsigned long long int newval, unsigned long long int oldval)
{
	double speed;
	speed = (newval - oldval) / 1024.0;
	if (speed > 1024.0) {
	    speed /= 1024.0;
	    sprintf(speedstr, "%6.3f MB", speed);
	} else {
	    sprintf(speedstr, "%6.2f KB", speed);
	}
}

char *
/* get_netusage(unsigned long long int *rec, unsigned long long int *sent) */
get_netusage()
{
	unsigned long long int newrec, newsent;
	newrec = newsent = 0;
	char downspeedstr[15], upspeedstr[15];
	/* static char retstr[42]; */
	int retval;

	retval = parse_netdev(&newrec, &newsent);
	if (retval) {
	    fprintf(stdout, "Error when parsing /proc/net/dev file.\n");
	    exit(1);
	}

	calculate_speed(downspeedstr, newrec, rec);
	calculate_speed(upspeedstr, newsent, sent);

	//↓↑ 
	/* sprintf(retstr, " %s %s \n", downspeedstr, upspeedstr); */
    char *retstr;
	retstr = smprintf(" ^c#9999FF^%s %s^d^ \n", downspeedstr, upspeedstr);

	rec = newrec;
	sent = newsent;
	return retstr;
}
