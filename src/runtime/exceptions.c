#include "ctransian/ctransian_runtime.h"
#include <setjmp.h>

// Simple exception/trap handling
static jmp_buf trap_jmp_buf;
static int trap_occurred = 0;

void ctransian_trap_setjmp(void) {
    trap_occurred = 0;
}

int ctransian_trap_occurred(void) {
    return trap_occurred;
}

void ctransian_trap_handler(const char* message) {
    (void)message;
    trap_occurred = 1;
    // In real implementation, would longjmp to trap_jmp_buf
}