#ifndef LAB1_MACROS_H
#define LAB1_MACROS_H

#define ALIGNMENT _Alignof(max_align_t)
#define SLAB_HEADER_SIZE align(sizeof (struct Slab), ALIGNMENT)
#define CACHE_HEADER_SIZE align(sizeof (struct Cache), ALIGNMENT)

#endif //LAB1_MACROS_H
