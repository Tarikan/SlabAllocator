#include <stdio.h>
#include <stddef.h>
#include "allocator.h"
#include "test.h"

int main() {

    void *res[40];
    for (int i = 0; i < 40; i++) {
        res[i] = mem_alloc(256);
    }

    for (int i = 0; i < 40; i++) {
        mem_free(res[i]);
    }

    test(256, 2000);

    return 0;
}
