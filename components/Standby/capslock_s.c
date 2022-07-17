#include <stdio.h>
/* #include <X11/XKBlib.h> */
#include <X11/Xlib.h>

char * capslock_s() {
    Display * d = XOpenDisplay((char*)0);
    XKeyboardState state;
    int isset;

    if (d) {
        /* unsigned n; */
        /* XkbGetIndicatorState(d, XkbUseCoreKbd, &n); */
        /* printf((n & 1)?"on\n":"off\n"); */
        /* return  ((n & 1)?" A\n":" a\n"); */
        XGetKeyboardControl(d, &state);
        XCloseDisplay(d);
        isset = (state.led_mask & 1);
        printf("Caps_L: %d", isset);
        return  ((isset & 1)?" A\n":" a\n");
    }
    return "";
}

/* int main() { */
/*     capslock_s(); */
/*     return 0; */
/* } */
