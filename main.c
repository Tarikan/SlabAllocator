#include <stdio.h>
#include <stddef.h>
#include "allocator.h"

int main() {
    int *test = mem_alloc(128);
    *test = 5;
    printf("%d\n", *test);

    test = mem_realloc(test, 256);

    printf("%d\n", *test);

    mem_free(test);

    return 0;
}
