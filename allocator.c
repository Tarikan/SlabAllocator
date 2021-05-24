#include <string.h>
#include "include/allocator.h"
#include <stddef.h>
#include "include/cache.h"
#include <stdbool.h>
#include "Utils/align_utils.h"
#include "include/mask.h"
#include "include/slab.h"

static bool first_cache_init = false;

static struct Cache root;

void InitRootCache() {
    first_cache_init = true;
    InitCache(&root, NULL, CACHE_HEADER_SIZE);
}

void *mem_alloc(size_t size) {
    size = align(size, ALIGNMENT);

    if (!first_cache_init) {
        InitRootCache();
    }

    struct Cache *cache = FindFirstFree(&root, size);
    struct Slab *slab;

    if (cache == NULL) {
        cache = InitCache(mem_alloc(align(sizeof(struct Cache), ALIGNMENT)),
                          GetLastCache(&root),
                          size);
    }

    if (cache->free_list == NULL) {
        slab = InitNewSlab(cache, 1);
    }
    else {
        slab = cache->free_list;
    }

    int bit = find_first_free(slab);

    SetBit(slab, bit);

    void *ptr = IndexToPtr(slab, bit);

    if (find_first_occupied(slab) == -1) {
        if (slab == cache->free_list) {
            cache->free_list = slab->next;
        }

        Append(Pop(slab), cache->occupied_list);
    }

    return ptr;
}

void mem_free(void *ptr) {
    struct Slab *slab = GetSlabFromObj(ptr);

    struct Cache* cache = FindFirstFree(&root, slab->obj_size);

    bool occupied = false;

    if (find_first_free(slab) == -1) {
        occupied = true;
    }

    int id = PtrToIndex(slab, ptr);

    UnsetBit(slab, id);

    if (occupied) {
        Append(Pop(slab), cache->free_list);
    }
}

void *mem_realloc(void *ptr, size_t new_size) {
    new_size = align(new_size, ALIGNMENT);

    if (ptr == NULL) {
        return mem_alloc(new_size);
    }

    struct Slab *slab = GetSlabFromObj(ptr);

    if (GetObjSize(slab) == new_size) {
        return ptr;
    }

    void *new_ptr = mem_alloc(new_size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr,
           GetObjSize(slab) > new_size ? new_size : GetObjSize(slab));

    mem_free(ptr);

    return new_ptr;
}
