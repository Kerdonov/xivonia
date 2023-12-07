#include "event.h"
#include "log.h"
#include "map.h"

#include <X11/Xlib.h>


void on_create_notify(const XCreateWindowEvent e) {}
void on_reparent_notify(const XReparentEvent e) {}
void on_map_notify(const XMapEvent e) {}
void on_configure_notify(const XConfigureEvent e) {}

void on_unmap_notify(WindowManager* wm, const XUnmapEvent e) {
    // return if window is not our client window
    if (!map_contains(wm->clients, e.window)) {
        simplelog("ignore UnmapNotify for non-client window %s", (char*)e.window);
        return;
    }
    // return if reparenting a pre-existing window
    if (e.event == wm->root) {
        simplelog("ignore UnmapNotify for reparented pre-existing window %s", (char*)e.window);
        return;
    }
    unframe(wm, e.window);
}

void on_destroy_notify(const XDestroyWindowEvent e) {}


void on_configure_request(WindowManager* wm, const XConfigureRequestEvent e) {
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;

    if (map_contains(wm->clients, e.window)) {
        Window frame = map_get(wm->clients, e.window);
        XConfigureWindow(wm->display, frame, e.value_mask, &changes);
        simplelog("resize [%s] to %dx%d\n", (char*)frame, e.width, e.height);
    }

    // grant request by calling XConfigureWindow()
    XConfigureWindow(wm->display, e.window, e.value_mask, &changes);
    simplelog("Resize %s to %dx%d", (char*)e.window, e.width, e.height);
}

void on_map_request(WindowManager* wm, const XMapRequestEvent e) {
    frame(wm, e.window, false);
}