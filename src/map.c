#include "map.h"
#include "log.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>

// todo please for the love of god
// ! do not write into *p_entry
// idk if this is my tiredness talking
// or is it altering my structure every time i run the loop
// what the fuck am i supposed to do then?

ClientMap* map_init(ClientMap* this) {
    this = malloc(sizeof(ClientMap));       // haha i forgor this line
    if (!this)
        on_malloc_failed();
    this->count = 0;
    this->cap = CLIENT_LIMIT;
    this->head = malloc(sizeof(struct client_entry*));
    *this->head = NULL;
    simplelog("MAP: initializaton complete");
    return this;
}


bool map_contains(ClientMap* this, Window w) {
    simplelog("MAP: map_contains with window %lu", w);
    struct client_entry* p_entry = this->head;
    while (p_entry) {
        if (p_entry->window == w)
            return true;
        p_entry = p_entry->next;
    }
    return false;
}

Window map_get(ClientMap* this, Window w) {
    simplelog("MAP: map_get with window %lu", w);
    struct client_entry** p_entry = this->head;
    while (*p_entry) {
        if ((*p_entry)->window == w)
            return (*p_entry)->frame;
        *p_entry = (*p_entry)->next;
    }
    errorlog("no such window %lu in ClientMap", w);
    return None;
}

void map_set(ClientMap* this, Window w, Window f) {
    struct client_entry** p_entry = this->head;
    while (*p_entry) {
        warninglog("wtf");
        // ClientMap already has this value, update
        if ((*p_entry)->window == w) {
            (*p_entry)->frame = f;
            return;
        }
        *p_entry = (*p_entry)->next;
    }
    // checks before adding a new item if there is room
    if (this->count >= this->cap) {
        errorlog("client limit of %u exceeded\n", this->cap);
        return;
    }
    // ClientMap doesn't have this value, add new
    *p_entry = malloc(sizeof(struct client_entry));
    if (!(*p_entry))
        on_malloc_failed();
    (*p_entry)->window = w;
    (*p_entry)->frame = f;
    (*p_entry)->next = NULL;
    this->count++;
    simplelog("MAP: map_set with window %lu and frame %lu", w, f);
}

void map_remove(ClientMap* this, Window w) {
    simplelog("MAP: map_remove with window %lu", w);
    struct client_entry* entry_to_remove = NULL;
    struct client_entry** p_entry = this->head;

    // remove first client_entry
    if ((*p_entry)->window == w) {
        struct client_entry* next_entry = (*p_entry)->next;
        free(*p_entry);
        (*p_entry) = next_entry;
        this->count--;
        return;
    }

    // remove other client_entry
    while ((*p_entry)->next) {
        if ((*p_entry)->next->window == w) {
            entry_to_remove = (*p_entry)->next;
            (*p_entry)->next = entry_to_remove->next;
            free(entry_to_remove);
            this->count--;
            return;
        }
        *p_entry = (*p_entry)->next;
    }
    errorlog("no such window %lu to remove from ClientMap", w);
}

