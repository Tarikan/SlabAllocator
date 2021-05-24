//
// Created by tarikan on 5/19/21.
//

#ifndef LAB2_OBJECT_H
#define LAB2_OBJECT_H

#include "slab.h"

struct Object {
    union {
        struct Slab *master;
        char data[sizeof(struct Slab *)];
    };
};

#endif //LAB2_OBJECT_H
