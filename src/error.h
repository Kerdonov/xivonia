#pragma once

#include <X11/Xlib.h>
#include <stdbool.h>

static bool wm_detected;

int on_wm_detected(Display* display, XErrorEvent* e);
int on_x_error(Display* display, XErrorEvent* e);
int on_malloc_failed();