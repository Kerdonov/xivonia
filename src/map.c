#include "map.h"
#include <stdlib.h>

Map* new_map() {
    Map* this = malloc(sizeof(this));
    this->cap = 8;
    this->len = 0;

    this->list = calloc((this->cap), sizeof(Pair*));
    return this;
}

void set(Map* this, Window w) {
    // todo
}

int contains(Map* m, Window w) {
    return 0;
}