#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include "../blocks.h"

static long int lastSum;
static long int lastIdle;

char *cpu_usage()
{
    char *ret;
	char str[100];
	const char d[2] = " ";
	char* token;
	int i = 0;
	long int sum = 0, idle;
	long double idleFraction;
 
    FILE* fp = fopen("/proc/stat","r");
    fgets(str,100,fp);
    fclose(fp);
    token = strtok(str,d);
    while(token!=NULL){
        token = strtok(NULL,d);
        if(token!=NULL){
            sum += atoi(token);

        if(i==3)
            idle = atoi(token);

        i++;
            }
        }
    idleFraction = 100 - (idle-lastIdle)*100.0/(sum-lastSum);
    /* ret = smprintf("Busy for : %lf %% of the time.\tlastIdle: %ld\tlastSum: %ld\n", (double)idleFraction, lastIdle, lastSum); */
    if (idleFraction > 80)
        ret = smprintf(" ^c#f7a8b8^ %2.0lf%%^d^ \n", (double)idleFraction);
    else
        ret = smprintf("  %2.0lf%% \n", (double)idleFraction);

    lastIdle = idle;
    lastSum = sum;
	return ret;
}

/* int main(void){ */
/*     char *p; */
/*     while (1) { */
/*         p = cpu_usage(); */
/*         printf("%s", p); */
/*         free(p); */
/*         sleep(1); */
/*     } */
/*     return 0; */
/* } */
