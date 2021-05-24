//
// Created by tarikan on 5/23/21.
//

#ifndef LAB2_MASK_H
#define LAB2_MASK_H

#include "slab.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "macros.h"
#include "../Utils/align_utils.h"

#define FREE 0
#define OCCUPIED 1

#define FREE_MASK = 0
#define OCCUPIED_MASK 0xFFFFFFFF

#define BITS_PER_LEVEL 32

void InitMask(struct Slab *slab);

void SetBit(struct Slab *slab, int bitNum);

void UnsetBit(struct Slab *slab, int bitNum);

bool TestBit(struct Slab *slab, int bitNum);

int find_first_unset(uint32_t v);

int find_first_free(struct Slab *slab);

int find_first_set(uint32_t v);

int find_first_occupied(struct Slab *slab);

void *IndexToPtr(struct Slab *slab, int index);
int PtrToIndex(struct Slab *slab, void *ptr);

#endif //LAB2_MASK_H
