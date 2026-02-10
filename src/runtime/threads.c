#include "ctransian/ctransian_runtime.h"
#include <pthread.h>

// Simple threading stub implementation
int ctransian_thread_create(ctransian_thread_t* thread, void (*func)(void*), void* arg) {
    // Placeholder - would use pthread_create in real implementation
    (void)thread; (void)func; (void)arg;
    return 0;
}

void ctransian_thread_join(ctransian_thread_t thread) {
    // Placeholder - would use pthread_join in real implementation
    (void)thread;
}