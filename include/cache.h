//
// Created by tarikan on 5/22/21.
//

#ifndef LAB2_CACHE_H
#define LAB2_CACHE_H

#include "stddef.h"
#include "slab.h"
#include "../Utils/align_utils.h"

struct Cache {
    size_t size;
    struct Cache *next, *prev;
    struct Slab *free_list, *occupied_list
};

struct Cache *InitCache(struct Cache *ptr, struct Cache *prev, size_t size);

void DestroyCache(struct Cache *cache);

struct Cache *FindFirstFree(struct Cache *cache, size_t size);

struct Slab *Pop(struct Slab* slab);

struct Slab *Append(struct Slab* slab, struct Slab* destination);

struct Cache *AppendCache(struct Cache* cache, struct Cache* destination);

struct Cache *GetLastCache(struct Cache* cache);

struct Slab *InitNewSlab(struct Cache *cache, int page_count);

#endif //LAB2_CACHE_H
