//
// Created by Tarikan on 09.03.2021.
//

#include "../include/driver.h"
#include <sys/mman.h>
#include <unistd.h>

static size_t page_size;

void *kernel_alloc(size_t size) {
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        return NULL;
    }

    return ptr;
}

void kernel_free(void *ptr) {
    int result = munmap(ptr, 0);
    if (result == -1) {
        printf("Munmap exited with error aborting");
        abort();
    }
}

size_t get_page_size() {
    if (!page_size) {
        page_size = sysconf(_SC_PAGESIZE);
    }
    return page_size;
}
