#pragma once

#include <X11/Xlib.h>



typedef struct window_manager {
    char* name;
    Display* display;
    // todo if needed make const with stupid memcpy hacks
    Window root;
    Window[2][10] clients;
} WindowManager;

// * wm functions
WindowManager* create_wm();
void close_wm(WindowManager* wm);
void run_wm(WindowManager* wm);


