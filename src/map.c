#include "map.h"
#include "log.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>

// ! forgive me lord for i have sinned

bool map_contains(ClientEntry* head, Window w) {
    simplelog("MAP: map_contains with window %lu", w);
    ClientEntry** p_entry = &head;
    while (*p_entry) {
        if ((*p_entry)->window == w)
            return true;
        p_entry = &(*p_entry)->next;
    }
    return false;
}

Window map_get(ClientEntry* head, Window w) {
    simplelog("MAP: map_get with window %lu", w);
    ClientEntry** p_entry = &head;
    while (*p_entry) {
        if ((*p_entry)->window == w)
            return (*p_entry)->frame;
        p_entry = &(*p_entry)->next;
    }
    errorlog("no such window %lu in ClientMap", w);
    return None;
}

void map_set(ClientEntry* head, Window w, Window f) {
    struct client_entry** p_entry = &head;
    while (*p_entry) {
        // ClientMap already has this value, update
        if ((*p_entry)->window == w) {
            (*p_entry)->frame = f;
            return;
        }
        p_entry = &(*p_entry)->next;
    }
    // ClientMap doesn't have this value, add new
    *p_entry = malloc(sizeof(struct client_entry));
    if (!(*p_entry))
        on_malloc_failed();
    (*p_entry)->window = w;
    (*p_entry)->frame = f;
    (*p_entry)->next = NULL;
    simplelog("MAP: map_set with window %lu and frame %lu", w, f);
}

void map_remove(ClientEntry* head, Window w) {
    simplelog("MAP: map_remove with window %lu", w);
    struct client_entry* entry_to_remove = NULL;
    struct client_entry** p_entry = &head;

    // remove first client_entry
    if ((*p_entry)->window == w) {
        ClientEntry* next_entry = (*p_entry)->next;
        free(*p_entry);
        (*p_entry) = next_entry;
        return;
    }

    // remove other client_entry
    while ((*p_entry)->next) {
        if ((*p_entry)->next->window == w) {
            entry_to_remove = (*p_entry)->next;
            (*p_entry)->next = entry_to_remove->next;
            free(entry_to_remove);
            return;
        }
        p_entry = &(*p_entry)->next;
    }
    errorlog("no such window %lu to remove from ClientMap", w);
}

