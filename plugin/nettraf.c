#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static unsigned long long int rec, sent;
//char *
//smprintf(char *fmt, ...)
//{
//	va_list fmtargs;
//	char *ret;
//	int len;
//
//	va_start(fmtargs, fmt);
//	len = vsnprintf(NULL, 0, fmt, fmtargs);
//	va_end(fmtargs);
//
//	ret = malloc(++len);
//	if (ret == NULL) {
//		perror("malloc");
//		exit(1);
//	}
//
//	va_start(fmtargs, fmt);
//	vsnprintf(ret, len, fmt, fmtargs);
//	va_end(fmtargs);
//
//	return ret;
//}

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
get_netusage(unsigned long long int *rec, unsigned long long int *sent)
{
	unsigned long long int newrec, newsent;
	newrec = newsent = 0;
	char downspeedstr[15], upspeedstr[15];
	static char retstr[42];
	int retval;

	retval = parse_netdev(&newrec, &newsent);
	if (retval) {
	    fprintf(stdout, "Error when parsing /proc/net/dev file.\n");
	    exit(1);
	}

	calculate_speed(downspeedstr, newrec, *rec);
	calculate_speed(upspeedstr, newsent, *sent);

	//↓↑ 
	sprintf(retstr, " %s %s \n", downspeedstr, upspeedstr);

	*rec = newrec;
	*sent = newsent;
	return retstr;
}

char *
show_nettraf(){
//    static unsigned long long int rec, sent;
//	  char *netstats;
//    netstats = get_netusage(&rec, &sent);
//    printf("%s\n",netstats);
    return get_netusage(&rec, &sent);
}

//int
//main(void)
//{
//	char *status;
//	char *netstats;
//	static unsigned long long int rec, sent;
//
//	parse_netdev(&rec, &sent);
//    for (;;) {
//        netstats = get_netusage(&rec, &sent);
////        status = smprintf("[N: %s]", netstats);
//        printf("%s\n",netstats);
////        free(status);
//        sleep(1);
//    }
//
//	return 0;
//}

