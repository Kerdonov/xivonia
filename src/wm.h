#pragma once

#include <X11/Xlib.h>
#include <stdbool.h>
#include "map.h"


// the main WindowManager struct
typedef struct window_manager {
    char* name;
    Display* display;
    // todo if needed make const with stupid memcpy hacks
    Window root;
    ClientMap* clients;
} WindowManager;


// * wm functions

// initializes the window manager, returns pointer
WindowManager* create_wm();
// closes the window manager
void close_wm(WindowManager* wm);
// check for another running wm + main event loop
void run_wm(WindowManager* wm);

void frame(WindowManager* wm, Window w, bool created_before_wm);
void unframe(WindowManager* wm, Window w);