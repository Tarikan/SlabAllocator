//
// Created by tarikan on 5/24/21.
//

#include "include/cache.h"

struct Cache *InitCache(struct Cache *ptr, struct Cache *prev, size_t size) {
    size = align(size, ALIGNMENT);
    ptr->size = size;
    ptr->prev = prev;
    if (prev != NULL) {
        prev->next = ptr;
    }
    ptr->next = NULL;
    ptr->free_list = NULL;
    ptr->occupied_list = NULL;

    return ptr;
}

void DestroyCache(struct Cache *cache) {
    if (cache->prev) {
        if (cache->next) {
            cache->prev->next = cache->next;
            cache->next->prev = cache->prev;
        }
        else {
            cache->prev->next = NULL;
        }
    }
    else {
        if (cache->next) {
            cache->next->prev = NULL;
        }
    }
    cache->next = cache->prev = NULL;
}

struct Cache *FindFirstFree(struct Cache *cache, size_t size) {
    while (cache) {
        if (cache->size == size) {
            return cache;
        }
        cache = cache->next;
    }
    return NULL;
}

struct Slab *Pop(struct Slab *slab) {
    if (GetNext(slab)) {
        if (GetPrev(slab)) {
            SetNext(GetPrev(slab), GetNext(slab));
            SetPrev(GetNext(slab), GetPrev(slab));
        }
        else {
            SetPrev(GetNext(slab), NULL);
        }
    }
    else {
        if (GetPrev(slab)) {
            SetNext(GetPrev(slab), NULL);
        }
    }

    SetNext(slab, NULL);
    SetPrev(slab, NULL);

    return slab;
}

struct Slab *Append(struct Slab *slab, struct Slab *destination) {
    destination = GetLastInList(destination);

    SetNext(slab, destination);
    SetPrev(destination, slab);
}

struct Slab *InitNewSlab(struct Cache *cache, int page_count) {
    struct Slab *slab = InitSlab(page_count,
                                 cache->size,
                                 GetLastInList(cache->free_list));

    if (cache->free_list == NULL) cache->free_list = slab;
    return slab;
}

struct Cache *AppendCache(struct Cache *cache, struct Cache *destination) {
    destination = GetLastCache(destination);
    destination->next = cache;
    cache->prev = destination;
}

struct Cache *GetLastCache(struct Cache *cache) {
    while (cache->next) {
        cache = cache->next;
    }

    return cache;
}
