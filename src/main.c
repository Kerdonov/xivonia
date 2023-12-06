#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "wm.h"
#include "ansi_colors.h"
#include "config.h"
#include "error.h"
#include "event.h"
#include "log.h"



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
            case ConfigureRequest:
                on_configure_request(wm, e.xconfigurerequest);
                break;
            case MapRequest:
                on_map_request(wm, e.xmaprequest);
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

void frame(WindowManager* wm, Window w) {
    XWindowAttributes x_window_attrs;
    if (XGetWindowAttributes(wm->display, w, &x_window_attrs) != 0)
        return;
    
    // todo framing existing top-level windows

    const Window frame = XCreateSimpleWindow(
        wm->display,
        wm->root,
        x_window_attrs.x,
        x_window_attrs.y,
        x_window_attrs.width,
        x_window_attrs.height,
        BORDER_WIDTH_PX,
        BORDER_COLOR,
        BG_COLOR
    );

    XSelectInput(
        wm->display,
        frame,
        SubstructureRedirectMask | SubstructureNotifyMask
    );

    XAddToSaveSet(wm->display, w);
    XReparentWindow(
        wm->display,
        w,
        frame,
        0, 0    // offset of client window within frame
    );
    XMapWindow(wm->display, frame);
    //wm->clients[w] = frame;
}


void close_wm(WindowManager* wm) {
    XCloseDisplay(wm->display);
}
