//
// Created by tarikan on 5/22/21.
//

#include "include/slab.h"
#include "Utils/align_utils.h"

struct Slab *InitSlab(unsigned int pages_count, size_t obj_size, struct Slab *prev) {
    struct Slab *ptr = (struct Slab *)kernel_alloc(pages_count * get_page_size());

    if (prev != NULL) {
        SetNext(prev, ptr);
        SetPrev(ptr, prev);
    }
    else {
        SetPrev(ptr, NULL);
    }
    SetObjSize(ptr, obj_size);

    ptr->obj_count = pages_count * get_page_size() / GetObjSize(ptr);

    InitMask(ptr);

    return ptr;
}

void DestroySlab(struct Slab *slab) {
    if (GetNext(slab)) {
        if (GetPrev(slab)) {
            SetPrev(GetNext(slab), GetPrev(slab));
            SetNext(GetPrev(slab), GetNext(slab));
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

    kernel_free(slab);
}

void SetNext(struct Slab *slab, void *next) {
    slab->next = next;
}

struct Slab *GetNext(struct Slab *slab) {
    return slab->next;
}

void SetPrev(struct Slab *slab, void *prev) {
    slab->prev = prev;
}

struct Slab *GetPrev(struct Slab *slab) {
    return slab->prev;
}

void SetObjSize(struct Slab *slab, size_t size) {
    slab->obj_size = size;
}

size_t GetObjSize(struct Slab *slab) {
    return slab->obj_size;
}

struct Slab *GetLastInList(struct Slab* slab) {
    if (slab == NULL) return NULL;
    while (GetNext(slab)) {
        slab = GetNext(slab);
    }

    return slab;
}

struct Slab *GetSlabFromObj(struct Object *obj) {
    return (struct Slab *)
            (align(((uintptr_t)obj), get_page_size()) - get_page_size());
}