//
// Created by tarikan on 5/23/21.
//

#include <assert.h>
#include "include/mask.h"

void InitMask(struct Slab *slab) {
    size_t mask_size = slab->obj_count / BITS_PER_LEVEL;
    if (mask_size * BITS_PER_LEVEL < slab->obj_count) ++mask_size;

    int header_size = (SLAB_HEADER_SIZE + mask_size) / slab->obj_size;
    if (header_size * slab->obj_size < (SLAB_HEADER_SIZE + mask_size)) ++header_size;

    assert(mask_size != 0);
    assert(header_size != 0);

    for (int i = 0; i < header_size; i++) {
        SetBit(slab, i);
    }
}

void SetBit(struct Slab *slab, int bitNum) {
    slab->mask[bitNum / BITS_PER_LEVEL] |= 1 << (bitNum % BITS_PER_LEVEL);
}

void UnsetBit(struct Slab *slab, int bitNum) {
    assert(bitNum != 0);
    slab->mask[bitNum / BITS_PER_LEVEL] &= ~(1 << (bitNum % BITS_PER_LEVEL));
}

bool TestBit(struct Slab *slab, int bitNum) {
    if ((slab->mask[bitNum / BITS_PER_LEVEL] & (1 << (bitNum % BITS_PER_LEVEL))) != 0) {
        // 1
        return true;
    }
    return false;
}

int find_first_unset(uint32_t v) {
    for(int pos = 0; pos < 32; pos++, v >>= 1) {
        if ((v & 1) == 0) return pos;
    }

    return -1;
}

int find_first_free(struct Slab *slab) {
    int res;
    int levels = BITS_PER_LEVEL / slab->obj_count;
    if (levels * slab->obj_count < BITS_PER_LEVEL) ++levels;
    for (int i = 0; i < levels; i++) {
        res = find_first_unset(slab->mask[i]);
        if (res >= slab->obj_count) {
            return -1;
        }
        if (res >= 0) {
            return res;
        }
    }
    return res;
}

int find_first_set(uint32_t v) {
    for(int pos = 0; pos < 32; pos++, v >>= 1) {
        if ((v & 1)) return pos;
    }

    return -1;
}

int find_first_occupied(struct Slab *slab) {
    int res;
    int levels = BITS_PER_LEVEL / slab->obj_count;
    if (levels * slab->obj_count < BITS_PER_LEVEL) ++levels;
    for (int i = 0; i < levels; i++) {
        res = find_first_unset(slab->mask[i]);
        if (res >= 0) {
            return res;
        }
    }
    return res;
}

void *IndexToPtr(struct Slab *slab, int index) {
    void* res;
    if (slab->obj_count * slab->obj_size > get_page_size()) {
        /// big object
        res = ((char *)slab) + index * (slab->obj_size + 2 * ALIGNMENT);
    }
    else res = ((char *)slab) + index * slab->obj_size;

    assert(slab != res);

    return res;
}

int PtrToIndex(struct Slab *slab, void *ptr) {
    /// Якщо об'єкт не вирівняний під 32, то це великий об'єкт
    //if (((uintptr_t) ptr) % (2 * ALIGNMENT) != 0) {
    //    return ((char*)ptr - (char*)slab) / (slab->obj_size + sizeof (struct Slab *));
    //}
    int res =  ((char*)ptr - (char*)slab) / slab->obj_size;

    assert(res != 0);

    return res;
}
