#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>

_Bool find_window(char* wname, Window* w_in){
    Display* d = XOpenDisplay(NULL);
    if(!d)puts("failed to find display");
    int n_s = ScreenCount(d);
    /* printf("%i %i\n", d->fd, n_s); */
    // Screen s = XDefaultScreen(d);
    Window w, gar, gari;
    Window* child_list;
    unsigned n_children;
    for(int i = 0; i < n_s; ++i){
        w = XRootWindow(d, i);
        if(!XQueryTree(d, w, &gar, &gari, &child_list, &n_children))
            puts("failed");
        printf("%i children\n", n_children);
        /* w = XRootWindow; */
    }
    XTextProperty tp;
    _Bool found = 0;
    for(unsigned i = 0; i < n_children; ++i){
        XGetWMName(d, child_list[i], &tp);
        if(tp.format == 8 && strstr((const char*)tp.value, wname)){
            found = 1;
            *w_in = child_list[i];
            puts((const char*)tp.value);
            break;
        }
    }
    Xfree(child_list);
    return found;
}

void update(Window w){
    XConfigureEvent ce;
    XWindowAttributes xwa;
    Display* d;
    XGetWindowAttributes((d = XOpenDisplay(NULL)), w, &xwa);

    ce.type = ConfigureNotify;
    ce.display = d;
    ce.event = w;
    ce.window = w;
    ce.x = xwa.x;
    ce.y = xwa.y;
    ce.width = xwa.width;
    printf("%i %i %i\n", ce.x, ce.y, ce.width);
    ce.border_width = xwa.border_width;
    printf("bw: %i\n", xwa.border_width);
    ce.above = None;
    ce.override_redirect = 0;

    XSendEvent(d, w, 0, StructureNotifyMask, (XEvent*)&ce);
    XSync(d, 0);
    /* xsendevent */
}

int main(int a, char** b){
    if(a < 2){
        printf("%s requires an argument\n", *b);
        return 0;
    }
    Window w;
    if(find_window(b[1], &w))
        puts("found window");
    else{
        puts("didn't find window");
        return 0;
    }
    update(w);
    Display* d = XOpenDisplay(NULL);
    printf("ret %i\n", XSetWindowBorderWidth(d, w, 9));
    printf("%i\n", XRaiseWindow(d, w));
    update(w);
    return 0;
}
