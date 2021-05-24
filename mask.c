//
// Created by tarikan on 5/23/21.
//

#include "include/mask.h"

void InitMask(struct Slab *slab) {
    int headerSize = SLAB_HEADER_SIZE / slab->obj_size;
    if (headerSize * slab->obj_size < SLAB_HEADER_SIZE) ++headerSize;
    for (int i = 0; i < headerSize; i++) {
        SetBit(slab, i);
    }
}

void SetBit(struct Slab *slab, int bitNum) {
    slab->mask[bitNum / BITS_PER_LEVEL] |= 1 << (bitNum % BITS_PER_LEVEL);
}

void UnsetBit(struct Slab *slab, int bitNum) {
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
    return ((char *)slab) + index * slab->obj_size;
}

int PtrToIndex(struct Slab *slab, void *ptr) {
    return ((char*)ptr - (char*)slab) / slab->obj_size;
}