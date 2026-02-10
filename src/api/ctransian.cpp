#include "ctransian/public.h"
#include "core/translator.h"
#include <memory>
#include <cstring>

extern "C" {

// Context management implementation
ctransian_context_t* ctransian_create(const ctransian_config_t* config) {
    if (!config) {
        return nullptr;
    }
    
    try {
        auto* translator = new ctransian::Translator(config);
        return reinterpret_cast<ctransian_context_t*>(translator);
    } catch (const std::exception&) {
        return nullptr;
    }
}

void ctransian_destroy_context(ctransian_context_t* ctx) {
    if (ctx) {
        auto* translator = reinterpret_cast<ctransian::Translator*>(ctx);
        delete translator;
    }
}

// Configuration helpers implementation
ctransian_config_t* ctransian_config_create(void) {
    auto* config = new ctransian_config_t();
    ctransian_config_set_defaults(config);
    return config;
}

void ctransian_config_destroy(ctransian_config_t* config) {
    delete config;
}

void ctransian_config_set_defaults(ctransian_config_t* config) {
    if (!config) return;
    
    // Zero out the structure first
    std::memset(config, 0, sizeof(ctransian_config_t));
    
    // Set default values
    config->optimization_level = CTRANIAN_OPTIMIZE_DEFAULT;
    config->enable_simd = false;
    config->enable_threads = false;
    config->enable_gc = false;
    config->enable_wasi = false;
    config->bounds_checking = CTRANIAN_BOUNDS_STRICT;
    config->target_c_standard = CTRANIAN_C11;
    config->runtime_type = CTRANIAN_RUNTIME_EMBEDDED;
    config->generate_debug_info = false;
    config->validate_input = true;
    config->generate_header = true;
    config->generate_comments = false;
}

// Translation functions implementation
ctransian_error_t ctransian_translate_binary(
    ctransian_context_t* ctx,
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_result_t* result
) {
    if (!ctx || !wasm_data || wasm_size == 0 || !result) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    ctransian_result_init(result);
    
    try {
        auto* translator = reinterpret_cast<ctransian::Translator*>(ctx);
        return translator->translateBinary(wasm_data, wasm_size, result);
    } catch (const std::exception& e) {
        result->error_code = CTRANIAN_ERROR_TRANSLATION_FAILED;
        result->error_message = strdup(e.what());
        return result->error_code;
    }
}

ctransian_error_t ctransian_translate_text(
    ctransian_context_t* ctx,
    const char* wat_text,
    ctransian_result_t* result
) {
    if (!ctx || !wat_text || !result) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    ctransian_result_init(result);
    
    try {
        auto* translator = reinterpret_cast<ctransian::Translator*>(ctx);
        return translator->translateText(wat_text, result);
    } catch (const std::exception& e) {
        result->error_code = CTRANIAN_ERROR_TRANSLATION_FAILED;
        result->error_message = strdup(e.what());
        return result->error_code;
    }
}

ctransian_error_t ctransian_translate_stream(
    ctransian_context_t* ctx,
    ctransian_read_callback_t read_cb,
    ctransian_write_callback_t write_cb,
    void* user_data
) {
    // Stream translation is not yet implemented
    return CTRANIAN_ERROR_UNSUPPORTED_FEATURE;
}

// Result management implementation
void ctransian_result_init(ctransian_result_t* result) {
    if (!result) return;
    
    std::memset(result, 0, sizeof(ctransian_result_t));
    result->error_code = CTRANIAN_SUCCESS;
}

void ctransian_result_cleanup(ctransian_result_t* result) {
    if (!result) return;
    
    if (result->error_message) {
        std::free(result->error_message);
        result->error_message = nullptr;
    }
    
    if (result->source_code) {
        std::free(result->source_code);
        result->source_code = nullptr;
    }
    
    if (result->header_code) {
        std::free(result->header_code);
        result->header_code = nullptr;
    }
    
    std::memset(result, 0, sizeof(ctransian_result_t));
}

const char* ctransian_error_string(ctransian_error_t error) {
    switch (error) {
        case CTRANIAN_SUCCESS:
            return "Success";
        case CTRANIAN_ERROR_INVALID_INPUT:
            return "Invalid input";
        case CTRANIAN_ERROR_INVALID_CONFIG:
            return "Invalid configuration";
        case CTRANIAN_ERROR_PARSE_FAILED:
            return "Failed to parse WebAssembly";
        case CTRANIAN_ERROR_TRANSLATION_FAILED:
            return "Translation failed";
        case CTRANIAN_ERROR_MEMORY_ALLOCATION:
            return "Memory allocation failed";
        case CTRANIAN_ERROR_UNSUPPORTED_FEATURE:
            return "Unsupported WebAssembly feature";
        case CTRANIAN_ERROR_VALIDATION_FAILED:
            return "WebAssembly validation failed";
        case CTRANIAN_ERROR_IO_ERROR:
            return "I/O error";
        default:
            return "Unknown error";
    }
}

// Version information implementation
const char* ctransian_version(void) {
    return CTRANIAN_VERSION;
}

uint32_t ctransian_version_number(void) {
    return CTRANIAN_VERSION_MAJOR * 10000 + 
           CTRANIAN_VERSION_MINOR * 100 + 
           CTRANIAN_VERSION_PATCH;
}

bool ctransian_has_feature(const char* feature_name) {
    if (!feature_name) return false;
    
#ifdef CTRANIAN_ENABLE_WASI
    if (std::strcmp(feature_name, "wasi") == 0) return true;
#endif

#ifdef CTRANIAN_ENABLE_GC
    if (std::strcmp(feature_name, "gc") == 0) return true;
#endif

#ifdef CTRANIAN_ENABLE_SIMD
    if (std::strcmp(feature_name, "simd") == 0) return true;
#endif

#ifdef CTRANIAN_ENABLE_THREADS
    if (std::strcmp(feature_name, "threads") == 0) return true;
#endif

    return false;
}

// Validation utilities implementation
ctransian_error_t ctransian_validate_binary(
    const uint8_t* wasm_data,
    size_t wasm_size
) {
    if (!wasm_data || wasm_size == 0) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Basic validation - actual WebAssembly validation would be implemented
    // using Binaryen's validation functions
    return CTRANIAN_SUCCESS;
}

ctransian_error_t ctransian_validate_text(const char* wat_text) {
    if (!wat_text) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Basic validation - actual WebAssembly validation would be implemented
    // using Binaryen's validation functions
    return CTRANIAN_SUCCESS;
}

// Analysis utilities implementation
ctransian_error_t ctransian_analyze_binary(
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_module_info_t* info
) {
    if (!wasm_data || wasm_size == 0 || !info) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Initialize info structure
    std::memset(info, 0, sizeof(ctransian_module_info_t));
    
    // Analysis would be implemented using Binaryen's analysis functions
    // For now, return a placeholder implementation
    return CTRANIAN_SUCCESS;
}

ctransian_error_t ctransian_analyze_text(
    const char* wat_text,
    ctransian_module_info_t* info
) {
    if (!wat_text || !info) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Initialize info structure
    std::memset(info, 0, sizeof(ctransian_module_info_t));
    
    // Analysis would be implemented using Binaryen's analysis functions
    // For now, return a placeholder implementation
    return CTRANIAN_SUCCESS;
}

} // extern "C"