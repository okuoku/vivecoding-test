#include <iostream>
#include <cassert>
#include "ctransian/public.h"

void test_config_creation() {
    std::cout << "Testing configuration creation..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    assert(config != nullptr);
    
    // Test default values
    assert(config->optimization_level == CTRANIAN_OPTIMIZE_DEFAULT);
    assert(config->enable_simd == false);
    assert(config->enable_threads == false);
    assert(config->bounds_checking == CTRANIAN_BOUNDS_STRICT);
    assert(config->target_c_standard == CTRANIAN_C11);
    
    ctransian_config_destroy(config);
    std::cout << "✓ Configuration creation test passed" << std::endl;
}

void test_context_creation() {
    std::cout << "Testing context creation..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian_context_t* ctx = ctransian_create(config);
    assert(ctx != nullptr);
    
    ctransian_destroy_context(ctx);
    ctransian_config_destroy(config);
    std::cout << "✓ Context creation test passed" << std::endl;
}

void test_version_info() {
    std::cout << "Testing version information..." << std::endl;
    
    const char* version = ctransian_version();
    assert(version != nullptr);
    std::cout << "Version: " << version << std::endl;
    
    uint32_t version_num = ctransian_version_number();
    assert(version_num > 0);
    std::cout << "Version number: " << version_num << std::endl;
    
    std::cout << "✓ Version information test passed" << std::endl;
}

void test_error_handling() {
    std::cout << "Testing error handling..." << std::endl;
    
    // Test error strings
    const char* success_msg = ctransian_error_string(CTRANIAN_SUCCESS);
    assert(success_msg != nullptr);
    std::cout << "Success message: " << success_msg << std::endl;
    
    const char* error_msg = ctransian_error_string(CTRANIAN_ERROR_INVALID_INPUT);
    assert(error_msg != nullptr);
    std::cout << "Error message: " << error_msg << std::endl;
    
    // Test result cleanup
    ctransian_result_t result;
    ctransian_result_init(&result);
    assert(result.error_code == CTRANIAN_SUCCESS);
    assert(result.source_code == nullptr);
    
    ctransian_result_cleanup(&result);
    std::cout << "✓ Error handling test passed" << std::endl;
}

void test_validation() {
    std::cout << "Testing validation..." << std::endl;
    
    // Test invalid input validation
    ctransian_error_t error = ctransian_validate_binary(nullptr, 0);
    assert(error == CTRANIAN_ERROR_INVALID_INPUT);
    
    error = ctransian_validate_text(nullptr);
    assert(error == CTRANIAN_ERROR_INVALID_INPUT);
    
    std::cout << "✓ Validation test passed" << std::endl;
}

int main() {
    std::cout << "Running ctransian basic tests..." << std::endl;
    std::cout << "=================================" << std::endl;
    
    try {
        test_config_creation();
        test_context_creation();
        test_version_info();
        test_error_handling();
        test_validation();
        
        std::cout << "=================================" << std::endl;
        std::cout << "✓ All basic tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "✗ Test failed with unknown exception" << std::endl;
        return 1;
    }
}