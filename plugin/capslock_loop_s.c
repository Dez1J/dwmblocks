/* gcc -lX11 -o capslock_s capslock_s.c */
#include <bits/types.h>
#include <stdio.h>
#include <X11/XKBlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc != 3) {
        printf("Usage: %s pid signum", argv[0]);
        exit(-1);
    }

    /* int pid; */
    int pid = atoi(argv[1]);
    // use strtol for error check,atoi func is unsafe
    int signum = atoi(argv[2]);
    printf("dwmblocks pid: %d\tsignum: %d\n", pid, signum);

    Display * d = XOpenDisplay((char*)0);

    if (d) {
        unsigned n,p = 2;

        /* kill(2337300, 47); // 34+defined_sig_in_block.h */
        while (1) {
            XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
            /* printf((n & 1)?" A\n":" a\n"); */
            /* printf("fetch info 'n': %u\n", n); */
            /* sleep(2); */
            if (p == n) {
                continue;
            }
            printf("p: %u,\t n: %u\n", p,n);
            printf((n & 1)?" A\n":" a\n");
            kill(pid, signum); // 34+defined_sig_in_block.h


            char *cap_l = (n & 1)?" A\n":" a\n";
            setenv("cap_l", cap_l, 1); // Restore old PATH
            /* system("echo $PATH"); // Outputs your actual PATH */
            system("notify-send 'Capslock' \"$cap_l\" ");

            p = n;
        }
        XCloseDisplay(d);
        printf("close programme!");
        return 0;
    }
    return -1;
}
