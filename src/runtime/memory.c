#include "ctransian/ctransian_runtime.h"
#include <stdlib.h>
#include <string.h>

// Simple memory management implementation
void* ctransian_memory_alloc(size_t size) {
    return malloc(size);
}

void ctransian_memory_free(void* ptr) {
    free(ptr);
}

bool ctransian_memory_check_bounds(const struct ctransian_memory* memory, uint32_t address, size_t size) {
    return (address + size <= memory->size);
}