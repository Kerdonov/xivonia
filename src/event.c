#include "event.h"
#include "log.h"


void on_create_notify(const XCreateWindowEvent e) {}

void on_configure_request(WindowManager* wm, const XConfigureRequestEvent e) {
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;
    // grant request by calling XConfigureWindow()
    XConfigureWindow(wm->display, e.window, e.value_mask, &changes);
    simplelog("Resize %s to %dx%d", (char*)e.window, e.width, e.height);
}

void on_map_request(WindowManager* wm, const XMapRequestEvent e) {
    // frame or re-frame window
    
}