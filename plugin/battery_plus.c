#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

#define PDN                             
#define PUP                             
#define LENGTH(X)                       (sizeof X / sizeof X[0])
#define BLOCKLENGTH                     512
char curtext[BLOCKLENGTH];
#define BLOCKLENGTH                     512
#define SPRINTF(str, ...)               ({ \
                                                int len = snprintf(str, BLOCKLENGTH, __VA_ARGS__); \
                                                len < BLOCKLENGTH ? len + 1 : BLOCKLENGTH; \
                                        })

#define NILL                            INT_MIN
#define ICONe                           "" /* unexpected error */
#define ICONa                           "" /* no battery */

#define ICON0                           ""
#define ICON1                           ""
#define ICON2                           ""
#define ICON3                           ""
#define ICON4                           ""
//#define ICON5                           ""
//#define ICON6                           ""
//#define ICON7                           ""
//#define ICON8                           ""
//#define ICON9                           ""

//#define ICON(bat)                       icons[(bat * (LENGTH(icons) - 1) + 50) / 100]
#define ICON(bat)                       icons[((bat - 1) * LENGTH(icons)) / 100 ]

#define BATC                            10 /* critical level */
#define BATL                            20 /* low level */
#define BATP                            40 /* plug in level */
#define BATU                            80 /* unplug level */

#define BATCAPFILE                      "/sys/class/power_supply/BAT0/capacity"
#define ACSTATEFILE                     "/sys/class/power_supply/AC0/online"
#define BATCFULLFILE                    "/sys/class/power_supply/BAT0/charge_full"
#define BATCNOWFILE                     "/sys/class/power_supply/BAT0/charge_now"
#define BATRATEFILE                     "/sys/class/power_supply/BAT0/current_now"

#define CNOTIFY(t, msg)                (char *[]){ "notify-send", \
                                                   "-h", "string:x-canonical-private-synchronous:batmon", \
                                                   "-u", "critical", \
                                                   "-t", t, \
                                                   "BatMon", msg, NULL }

#define NNOTIFY(t, msg)                (char *[]){ "notify-send", \
                                                   "-h", "string:x-canonical-private-synchronous:batmon", \
                                                   "-t", t, \
                                                   "BatMon", msg, NULL }

#define TNOTIFY(t, msg)                (char *[]){ "notify-send", \
                                                   "-h", "string:x-canonical-private-synchronous:batmon", \
                                                   "-h", "int:transient:1", \
                                                   "-t", t, \
                                                   "BatMon", msg, NULL }

#define UCNOTIFY(t, msg)                uspawn(NNOTIFY(t, msg))
#define UNNOTIFY(t, msg)                uspawn(NNOTIFY(t, msg))
#define UTNOTIFY(t, msg)                uspawn(TNOTIFY(t, msg))

enum { Normal, Critical, Low, Plug, Unplug };

int
readint(const char *path, int *var) {
        FILE *fp;

        if (!(fp = fopen(path, "r")))
                return 0;
        if (fscanf(fp, "%d", var) != 1) {
                fclose(fp);
                return 0;
        }
        fclose(fp);
        return 1;
}

Display *dpy;
void
cleanup()
{
        /* XStoreName(dpy, DefaultRootWindow(dpy), ""); */
    XCloseDisplay(dpy);
}
void
uspawn(char *const *arg)
{
        switch (fork()) {
                case -1:
                        perror("uspawn - fork");
                        cleanup();
                        exit(1);
                case 0:
                        close(ConnectionNumber(dpy));
                        setsid();
                        execvp(arg[0], arg);
                        perror("uspawn - child - execvp");
                        _exit(127);
        }
}
size_t
batteryu(char *str, int ac)
{
        static int level = Normal;
        static char *icons[] = { ICON0, ICON1, ICON2, ICON3, ICON4 };
        int bat;

        if (!readint(BATCAPFILE, &bat)) {
                strcpy(str, ICONa);
                return sizeof ICONa;
        }
        /* routine update */
        if (ac == NILL) {
                if (!readint(ACSTATEFILE, &ac))
                        return SPRINTF(str, ICONe "%d%%", bat);
                if (ac) {
                        if (bat < BATU)
                                level = Normal;
                        else {
                                if (level != Unplug) {
                                        UNNOTIFY("0", "Unplug the charger");
                                        level = Unplug;
                                }
                        }
                        return SPRINTF(str, PUP "%s%d%%", ICON(bat), bat);
                } else {
                        if (bat > BATP)
                                level = Normal;
                        else if (bat > BATL) {
                                if (level != Plug) {
                                        UNNOTIFY("0", "Plug in the charger");
                                        level = Plug;
                                }
                        } else if (bat > BATC) {
                                if (level != Low) {
                                        UNNOTIFY("0", "Battery level is low!");
                                        level = Low;
                                }
                        } else {
                                if (level != Critical) {
                                        UCNOTIFY("0", "Battery level is critical!");
                                        level = Critical;
                                }
                        }
                        return SPRINTF(str, PDN "%s%d%%", ICON(bat), bat);
                }
        /* charger plugged in */
        } else if (ac) {
                if (bat < BATU) {
                        UTNOTIFY("1000", "Charger plugged in");
                        level = Normal;
                } else {
                        //UNNOTIFY("0", "Unplug the charger");
                        UNNOTIFY("0", "please Unplug the charger");
                        level = Unplug;
                }
                return SPRINTF(str, PUP "%s%d%%", ICON(bat), bat);
        /* charger plugged out */
        } else {
                if (bat > BATP) {
                        UTNOTIFY("1000", "Charger plugged out");
                        level = Normal;
                } else if (bat > BATL) {
                        UNNOTIFY("0", "Plug in the charger");
                        level = Plug;
                } else if (bat > BATC) {
                        UNNOTIFY("0", "Battery level is low!");
                        level = Low;
                } else {
                        UCNOTIFY("0", "Battery level is critical!");
                        level = Critical;
                }
                return SPRINTF(str, PDN "%s%d%%", ICON(bat), bat);
        }
}

int main(){
    if (!(dpy = XOpenDisplay(NULL))) {
        fputs("Error: could not open display.\n", stderr);
        return 1;
    }
    batteryu(curtext, 1);
//    uspawn(TNOTIFY("0", "main function x notify information senf test!"));
    printf("battery test:%s",curtext);
    XCloseDisplay(dpy);
    return 0;
}
