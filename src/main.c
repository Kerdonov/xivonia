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
#include "map.h"



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

    XSetErrorHandler(on_x_error);
    
    // grab x server to prevent windows from changing while framing them
    XGrabServer(wm->display);
    // frame existing top level windows
    Window returned_root, returned_parent;
    Window* top_level_windows;
    unsigned int num_top_level_windows;
    if (!XQueryTree(
        wm->display,
        wm->root,
        &returned_root,
        &returned_parent,
        &top_level_windows,
        &num_top_level_windows)) {
        errorlog("XQueryTree failed");
        close_wm(wm);
    }
    if (returned_root != wm->root) {
        errorlog("XQueryTree didn't return expected root window");
        close_wm(wm);
    }
    for (unsigned int i = 0; i < num_top_level_windows; ++i) {
        frame(wm, top_level_windows[i], true);
    }
    XFree(top_level_windows);
    XUngrabServer(wm->display);

    // main event loop
    for (;;) {
        // get event
        XEvent e;
        XNextEvent(wm->display, &e);
        simplelog("received event: %d\n", e.type);

        // dispatch event
        switch (e.type) {
            // * notifications
            case CreateNotify:
                on_create_notify(e.xcreatewindow);
                break;
            case ReparentNotify:
                on_reparent_notify(e.xreparent);
                break;
            case MapNotify:
                on_map_notify(e.xmap);
                break;
            case ConfigureNotify:
                on_configure_notify(e.xconfigure);
                break;
            case UnmapNotify:
                on_unmap_notify(wm, e.xunmap);
                break;
            case DestroyNotify:
                on_destroy_notify(e.xdestroywindow);
                break;
            
            // * requests
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
    char* name = WM_NAME;
    if (display == NULL) {
        errorlog("failed to open x display\n");
        return NULL;
    }

    // construct WindowManager instance (wm)
    WindowManager* wm = malloc(sizeof(WindowManager));
    wm->display = display;
    wm->name = name;
    wm->root = DefaultRootWindow(wm->display);
    map_init(wm->clients);
    return wm;
}



void frame(WindowManager* wm, Window w, bool created_before_wm) {
    XWindowAttributes x_window_attrs;
    if (XGetWindowAttributes(wm->display, w, &x_window_attrs) != 0)
        return;
    
    if (created_before_wm) {
        if (x_window_attrs.override_redirect ||
            x_window_attrs.map_state != IsViewable) {
            return;
        }
    }

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
    map_set(wm->clients, w, frame);

    // todo XGrabButton() and XGrabKey stuff

    simplelog("framed window %s [%s]\n", (char*)w, (char*)frame);
}



void unframe(WindowManager* wm, Window w) {
    Window frame = map_get(wm->clients, w);
    map_remove(wm->clients, w);     // ? i think this should be here
    XUnmapWindow(wm->display, frame);

    // reparent window to root window
    XReparentWindow(
        wm->display,
        w,
        wm->root,
        0, 0);      // offset within root
    
    XRemoveFromSaveSet(wm->display, w);
    XDestroyWindow(wm->display, frame);
    // ? and not here

    simplelog("unframed window %lu [%lu]", w, frame);
}


void close_wm(WindowManager* wm) {
    simplelog("exiting wm");
    free(wm);
    XCloseDisplay(wm->display);
}
