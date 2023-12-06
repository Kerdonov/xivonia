#pragma once

#include <X11/Xlib.h>
#include "wm.h"

void on_create_notify(const XCreateWindowEvent e);
void on_configure_request(WindowManager* wm, const XConfigureRequestEvent e);
void on_map_request(WindowManager* wm, const XMapRequestEvent e);
