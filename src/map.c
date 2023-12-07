#include "map.h"
#include "log.h"
#include <stdlib.h>
#include <stdbool.h>


void map_init(ClientMap* this) {
    simplelog("initialize map\n");
    this = malloc(sizeof(ClientMap));       // haha i forgor this line
    this->count = 0;
    this->cap = CLIENT_LIMIT;
    this->head = malloc(sizeof(struct client_entry*));
    *this->head = NULL;
    simplelog("initializaton complete\n");
}


bool map_contains(ClientMap* this, Window w) {
    struct client_entry* p_entry = *(this->head);
    while (p_entry) {
        if (p_entry->window == w)
            return true;
        p_entry = p_entry->next;
    }
    return false;
}

Window map_get(ClientMap* this, Window w) {
    struct client_entry* p_entry = *(this->head);
    while (p_entry) {
        if (p_entry->window == w)
            return p_entry->frame;
        p_entry = p_entry->next;
    }
    errorlog("no such window %lu in ClientMap", w);
    return None;
}

void map_set(ClientMap* this, Window w, Window f) {
    struct client_entry* p_entry = *(this->head);
    for (;;) {
        // ClientMap already has this value, update
        if (p_entry->window == w) {
            p_entry->frame = f;
            return;
        }
        if (!p_entry->next) break;
        p_entry = p_entry->next;
    }
    if (this->count >= this->cap) {
        errorlog("client limit of %u exceeded\n", this->cap);
        return;
    }
    // ClientMap doesn't have this value, add new
    p_entry->next = malloc(sizeof(struct client_entry));
    p_entry->next->window = w;
    p_entry->next->frame = f;
    p_entry->next->next = NULL;
    this->count++;
}

void map_remove(ClientMap* this, Window w) {
    struct client_entry* entry_to_remove = NULL;
    struct client_entry* p_entry = *(this->head);

    // remove first client_entry
    if (p_entry->window == w) {
        struct client_entry* next_entry = p_entry->next;
        free(p_entry);
        p_entry = next_entry;
        this->count--;
        return;
    }

    // remove other client_entry
    while (p_entry->next) {
        if (p_entry->next->window == w) {
            entry_to_remove = p_entry->next;
            p_entry->next = entry_to_remove->next;
            free(entry_to_remove);
            this->count--;
            return;
        }
        p_entry = p_entry->next;
    }
    errorlog("no such window %lu to remove from ClientMap\n", w);
}

