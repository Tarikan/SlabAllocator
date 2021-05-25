#include <stdio.h>
#include <stddef.h>
#include "allocator.h"
#include "test.h"

int main() {

    //void *super_test = mem_alloc(1024);
    //mem_free(super_test);

    void *res[40];
    for (int i = 0; i < 40; i++) {
        res[i] = mem_alloc(2000);
    }

    for (int i = 0; i < 40; i++) {
        res[i] = mem_realloc(res[i], 3000);
    }

    for (int i = 0; i < 40; i++) {
        mem_free(res[i]);
    }

    test(2048, 2000);

    return 0;
}
