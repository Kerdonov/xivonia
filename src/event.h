#pragma once

#include <X11/Xlib.h>
#include "wm.h"


// * nofity handlers

void on_create_notify(const XCreateWindowEvent e);
void on_reparent_notify(const XReparentEvent e);
void on_map_notify(const XMapEvent e);
void on_configure_notify(const XConfigureEvent e);
void on_unmap_notify(WindowManager* wm, const XUnmapEvent e);
void on_destroy_notify(const XDestroyWindowEvent e);

// * request handlers
void on_configure_request(WindowManager* wm, const XConfigureRequestEvent e);
void on_map_request(WindowManager* wm, const XMapRequestEvent e);
