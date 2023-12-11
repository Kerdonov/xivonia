#pragma once

#include <X11/Xlib.h>
#include "config.h"
#include <stdbool.h>



typedef struct client_entry {
    Window window;
    Window frame;
    struct client_entry* next;
} ClientEntry;



// checks if entry with key w exists in ClientMap
bool map_contains(ClientEntry* head, Window w);
// returns the corresponding frame
Window map_get(ClientEntry* head, Window w);
// adds or updates the ClientMap
void map_set(ClientEntry* head, Window w, Window f);
// removes the value with this key from the ClientMap
void map_remove(ClientEntry* head, Window w);