#include <stdio.h>
#include <X11/XKBlib.h>

char * capslock_s() {
    Display * d = XOpenDisplay((char*)0);

    if (d) {
        unsigned n;

        XkbGetIndicatorState(d, XkbUseCoreKbd, &n);

        /* printf((n & 1)?"on\n":"off\n"); */
        XCloseDisplay(d);
        return  ((n & 1)?" A\n":" a\n");
    }
    return "";
}
