//
// Created by tarikan on 5/19/21.
//

#ifndef LAB2_SLAB_H
#define LAB2_SLAB_H

#include "driver.h"
#include "stddef.h"
#include "mask.h"
#include "object.h"

struct Slab {
    struct Slab *prev, *next;
    size_t obj_size;
    unsigned short obj_count;
    unsigned int mask[];
};

struct Slab* InitSlab(unsigned int pages_count, size_t obj_size, struct Slab *prev);
void DestroySlab(struct Slab *slab);

void SetNext(struct Slab* slab, void *next);
struct Slab *GetNext(struct Slab* slab);

void SetPrev(struct Slab *slab, void *prev);
struct Slab *GetPrev(struct Slab *slab);

void SetObjSize(struct Slab* slab, size_t size);
size_t GetObjSize(struct Slab* slab);

struct Slab *GetLastInList(struct Slab* slab);

struct Slab *GetSlabFromObj(struct Object *obj);


#endif //LAB2_SLAB_H
