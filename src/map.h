#pragma once

#include <X11/Xlib.h>
#include "config.h"
#include <stdbool.h>




struct client_entry {
    Window window;
    Window frame;
    struct client_entry* next;
};

typedef struct client_map{
    unsigned int count;
    unsigned int cap;
    struct client_entry** head;
} ClientMap;



// initializes the ClientMap
void map_init(ClientMap* this);
// checks if entry with key w exists in ClientMap
bool map_contains(ClientMap* this, Window w);
// returns the corresponding frame
Window map_get(ClientMap* this, Window w);
// adds or updates the ClientMap
void map_set(ClientMap* this, Window w, Window f);
// removes the value with this key from the ClientMap
void map_remove(ClientMap* this, Window w);