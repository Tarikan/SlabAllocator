#include <string.h>
#include "include/allocator.h"
#include <stddef.h>
#include "include/cache.h"
#include <stdbool.h>
#include <assert.h>
#include "Utils/align_utils.h"
#include "include/mask.h"
#include "include/slab.h"
#include <math.h>

static bool first_cache_init = false;

static struct Cache root;

void InitRootCache() {
    first_cache_init = true;
    InitCache(&root, NULL, CACHE_HEADER_SIZE);
}

void *mem_alloc(size_t size) {
    bool big_slab = false;

    bool cache_initted = false;
    bool slab_initted = false;


    int page_count = 1;
    size = align(size, 2 * ALIGNMENT);

    if (!first_cache_init) {
        InitRootCache();
    }

    if (get_page_size() / size < 8) {
        /// Big slab
        big_slab = true;
        page_count = align(size * 8, get_page_size()) / get_page_size();
    }

    struct Cache *cache = FindFirst(&root, !big_slab ? size : size + 2 * ALIGNMENT);
    struct Slab *slab;

    if (cache == NULL) {
        cache = InitCache(mem_alloc(CACHE_HEADER_SIZE),
                          GetLastCache(&root),
                          !big_slab ? size : size + 2 * ALIGNMENT);
        cache_initted = true;
    }

    if (cache->free_list == NULL) {
        slab = InitNewSlab(cache, page_count);
        slab_initted = true;
    }
    else {
        slab = cache->free_list;
    }

    int bit = find_first_free(slab);

    SetBit(slab, bit);

    void *ptr = IndexToPtr(slab, bit);
    if (big_slab) {
        ((union body *)ptr)->master = slab;

        assert(((uintptr_t) ptr) % (2 * ALIGNMENT) == 0);

        ptr = (char *)ptr + ALIGNMENT;

        assert(((uintptr_t) ptr) % (2 * ALIGNMENT) != 0);
    }

    if (find_first_free(slab) == -1) {
        struct Slab *next = GetNext(slab);

        if (cache->occupied_list != NULL) {
            Append(Pop(slab), cache->occupied_list);
        }
        else {
            cache->occupied_list = Pop(slab);
        }

        if (slab == cache->free_list) {
            cache->free_list = next;
        }
    }

    assert(ptr != slab);

    return ptr;
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    struct Slab *slab = GetSlabFromObj(ptr);

    struct Cache *cache = FindFirst(&root, slab->obj_size);

    bool occupied = false;

    if (find_first_free(slab) == -1) {
        occupied = true;
    }

    int id = PtrToIndex(slab, ptr);

    UnsetBit(slab, id);

    if (occupied) {
        struct Slab* next = GetNext(slab);

        if (cache->free_list != NULL) {
            Append(Pop(slab), cache->free_list);
        }
        else {
            cache->free_list = Pop(slab);
        }
        if (cache->occupied_list == slab) {
            cache->occupied_list = next;
        }
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
