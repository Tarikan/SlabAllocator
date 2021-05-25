//
// Created by tarikan on 5/19/21.
//

#ifndef LAB2_OBJECT_H
#define LAB2_OBJECT_H

#include "slab.h"

struct Object {
    /// Якщо об'єкт вирівняний по 32, то це точно малий об'єкт (мінімальний розмір алокації - 32 байта)
    union body {
        struct Slab *master;
        char data[sizeof(struct Slab *)];
    };
};

#endif //LAB2_OBJECT_H
