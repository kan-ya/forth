#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline void *not_null(void *ptr) {
    if (ptr == nullptr) {
        fprintf(stderr, "failed to memory allocate\n");
        exit(1);
    }
    return ptr;
}
#define alloc(Type) calloc(1, sizeof(Type))
#define new(Type) not_null(alloc(Type))
