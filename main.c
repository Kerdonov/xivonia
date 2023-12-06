#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "colors.h"

static bool wm_detected = false;



int main() {
    WindowManager* wm = create_wm();

    if (!wm) {
        errorlog("failed to initialize window manager\n");
        return 1;
    }

    run_wm(wm);

    return 0;
}



// * wm functions

void run_wm(WindowManager* wm) {
    simplelog("running wm: %s\n", wm->name);

    // initialization
    XSetErrorHandler(on_wm_detected);
    XSelectInput(
        wm->display,
        wm->root,
        SubstructureRedirectMask | SubstructureNotifyMask);
    XSync(wm->display, false);
    if (wm_detected) {
        errorlog("detected another window manager on display\n");
        return;
    }
    simplelog("wm detected: %d\n", wm_detected);

    XSetErrorHandler(on_x_error);

    // main event loop
    for (;;) {
        // get event
        XEvent e;
        XNextEvent(wm->display, &e);
        simplelog("received event: %d\n", e.type);

        // dispatch event
        switch (e.type) {
            case CreateNotify:
                on_create_notify(e.xcreatewindow);
                break;
            case DestroyNotify:
                on_destroy_notify(e.xdestroywindow);
                break;
            case ReparentNotify:
                on_reparent_notify(e.xreparent);
                break;
            
            default:
                warninglog("ignored event");
        }
    }
}


WindowManager* create_wm() {
    // open x display
    Display* display = XOpenDisplay(NULL);
    char* name = "xivonia";
    if (display == NULL) {
        errorlog("failed to open x display\n");
        return NULL;
    }

    // construct WindowManager instance (wm)
    WindowManager* wm = malloc(sizeof(WindowManager));
    wm->display = display;
    wm->name = name;
    wm->root = DefaultRootWindow(wm->display);
    return wm;
}


void close_wm(WindowManager* wm) {
    XCloseDisplay(wm->display);
}






// ! error handlers

int on_wm_detected(Display* display, XErrorEvent* e) {
    if ((int)e->error_code != BadAccess)
        return 0;
    wm_detected = true;
    return 0;
}

int on_x_error(Display* display, XErrorEvent* e) {
    errorlog("x error: %d\n", e->error_code);
    return 0;
}