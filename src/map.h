#pragma once

#include <X11/Xlib.h>

typedef struct pair {
    Window w;
    const Window f;
    struct pair* next;
} Pair;

typedef struct map {
    Pair** list;
    unsigned int cap;
    unsigned int len;
} Map;

Map* new_map();
int contains(Map* m, Window w);