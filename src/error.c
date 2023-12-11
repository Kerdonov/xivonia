#include "error.h"
#include "log.h"
#include <X11/Xlib.h>
#include <stdbool.h>

static bool wm_detected = false;


int on_wm_detected(Display* display, XErrorEvent* e) {
    if ((int)e->error_code != BadAccess)
        return 0;
    wm_detected = true;
    return 0;
}

int on_x_error(Display* display, XErrorEvent* e) {
    errorlog("x error: %d", e->error_code);
    return 0;
}

int on_malloc_failed() {
    errorlog("malloc failed");
    return 0;
}