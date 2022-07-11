#define BATT_NOW        "/sys/class/power_supply/BAT0/charge_now"
#define BATT_FULL       "/sys/class/power_supply/BAT0/charge_full"
#define BATT_STATUS       "/sys/class/power_supply/BAT0/status"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../utils.h"

char *
getbattery(){
    long lnum1, lnum2 = 0;
    char *status = malloc(sizeof(char)*12);
    char s = '?';
    FILE *fp = NULL;
    char *ret;
    if ((fp = fopen(BATT_NOW, "r"))) {
        fscanf(fp, "%ld\n", &lnum1);
        fclose(fp);
        fp = fopen(BATT_FULL, "r");
        fscanf(fp, "%ld\n", &lnum2);
        fclose(fp);
        fp = fopen(BATT_STATUS, "r");
        fscanf(fp, "%s\n", status);
        fclose(fp);
        if (strcmp(status,"Charging") == 0)
            s = '+';
        if (strcmp(status,"Discharging") == 0)
            s = '-';
        if (strcmp(status,"Full") == 0)
            s = '=';
        ret = smprintf("^c#98FB98^  %c%ld%%^d^\n", s, (lnum1/(lnum2/100)) );
        /* printf("%s\n", ret); */
        /* printf("%d\n",(int)strlen(ret)); */
        /* return smprintf("%c%ld%% \n", s,(lnum1/(lnum2/100))); */
    }
    /* else return smprintf(""); */
    else
        ret = smprintf("Err\n");
    return ret;
}
