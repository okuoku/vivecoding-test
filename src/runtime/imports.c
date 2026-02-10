#include "ctransian/ctransian_runtime.h"

// Simple import function handling
void* ctransian_import_resolve(const char* module_name, const char* field_name) {
    (void)module_name; (void)field_name;
    // Placeholder - would look up imported functions
    return NULL;
}