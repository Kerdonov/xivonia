#include "event.h"
#include "log.h"
#include "map.h"
#include "wm.h"

#include <X11/Xlib.h>


void on_create_notify(const XCreateWindowEvent e) {
    simplelog("createnotify window %lu", e.window);
}
void on_reparent_notify(const XReparentEvent e) {
    simplelog("reparentnotify window %lu", e.window);
}
void on_map_notify(const XMapEvent e) {
    simplelog("mapnotify window %lu", e.window);
}
void on_configure_notify(const XConfigureEvent e) {
    simplelog("configurenotify window %lu", e.window);
}

void on_unmap_notify(WindowManager* wm, const XUnmapEvent e) {
    simplelog("unmapnotify window %lu", e.window);
    // return if window is not our client window
    if (!map_contains(wm->head, e.window)) {
        simplelog("ignore UnmapNotify for non-client window %lu", e.window);
        return;
    }
    // return if reparenting a pre-existing window
    if (e.event == wm->root) {
        simplelog("ignore UnmapNotify for reparented pre-existing window %lu", e.window);
        return;
    }
    unframe(wm, e.window);
}

void on_destroy_notify(WindowManager* wm, const XDestroyWindowEvent e) {
    simplelog("destroynotify window %lu", e.window);
    //unframe(wm, e.window);
}


void on_configure_request(WindowManager* wm, const XConfigureRequestEvent e) {
    simplelog("configurerequest window %lu", e.window);
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;

    if (map_contains(wm->head, e.window)) {
        Window frame = map_get(wm->head, e.window);
        XConfigureWindow(wm->display, frame, e.value_mask, &changes);
        simplelog("resize [%lu] to %dx%d", frame, e.width, e.height);
    }

    // grant request by calling XConfigureWindow()
    simplelog("grant configurerequest for unframed window");
    XConfigureWindow(wm->display, e.window, e.value_mask, &changes);
    simplelog("Resize %lu to %dx%d", e.window, e.width, e.height);
}

void on_map_request(WindowManager* wm, const XMapRequestEvent e) {
    simplelog("maprequest window %lu", e.window);
    frame(wm, e.window, false);
}