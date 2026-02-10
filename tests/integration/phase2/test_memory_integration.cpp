#include "../utils/test_helpers.h"
#include <iostream>

using namespace ctransian_test;

// Test conversion of memory operations test file
TestResult testMemoryConversion() {
    std::string watFile = "test_data/phase2/memory/memory_operations.wat";
    std::string wasmFile = "test_data/phase2/memory/memory_operations.wasm";
    
    TestResult result = convertWatToWasm(watFile, wasmFile);
    if (!result.success) {
        return TestResult(false, "Failed to convert memory operations test file: " + result.message, "");
    }
    
    return TestResult(true, "Memory operations test file converted successfully", "");
}

// Test i32 store/load operation
TestResult testI32StoreLoad() {
    TestResult result = executeWasmFile("test_data/phase2/memory/memory_operations.wasm", "test_i32_store_load");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_store_load: " + result.message, result.output);
    }
    
    // Should return 42 (the value that was stored and loaded)
    if (result.output.find("42") == std::string::npos) {
        return TestResult(false, "Expected output to contain 42, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32 store/load test passed", result.output);
}

// Test i64 store/load operation
TestResult testI64StoreLoad() {
    TestResult result = executeWasmFile("test_data/phase2/memory/memory_operations.wasm", "test_i64_store_load");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i64_store_load: " + result.message, result.output);
    }
    
    // Should return 12345678900 (the value that was stored and loaded)
    if (result.output.find("12345678900") == std::string::npos) {
        return TestResult(false, "Expected output to contain 12345678900, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i64 store/load test passed", result.output);
}

// Test f32 store/load operation
TestResult testF32StoreLoad() {
    TestResult result = executeWasmFile("test_data/phase2/memory/memory_operations.wasm", "test_f32_store_load");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f32_store_load: " + result.message, result.output);
    }
    
    // Should return a value containing 3.14159 (the float that was stored and loaded)
    if (result.output.find("3.14159") == std::string::npos) {
        return TestResult(false, "Expected output to contain 3.14159, got: " + result.output, result.output);
    }
    
    return TestResult(true, "f32 store/load test passed", result.output);
}

// Test f64 store/load operation
TestResult testF64StoreLoad() {
    TestResult result = executeWasmFile("test_data/phase2/memory/memory_operations.wasm", "test_f64_store_load");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f64_store_load: " + result.message, result.output);
    }
    
    // Should return a value containing 2.718281828459045 (the double that was stored and loaded)
    if (result.output.find("2.718281828459045") == std::string::npos) {
        return TestResult(false, "Expected output to contain 2.718281828459045, got: " + result.output, result.output);
    }
    
    return TestResult(true, "f64 store/load test passed", result.output);
}

// Test offset store/load operation
TestResult testOffsetStoreLoad() {
    TestResult result = executeWasmFile("test_data/phase2/memory/memory_operations.wasm", "test_offset_store_load");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_offset_store_load: " + result.message, result.output);
    }
    
    // Should return 100 (the value that was stored at offset 100 and loaded)
    if (result.output.find("100") == std::string::npos) {
        return TestResult(false, "Expected output to contain 100, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Offset store/load test passed", result.output);
}

// Test multiple operations
TestResult testMultipleOperations() {
    TestResult result = executeWasmFile("test_data/phase2/memory/memory_operations.wasm", "test_multiple_operations");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_multiple_operations: " + result.message, result.output);
    }
    
    // Should return 60 (10 + 20 + 30)
    if (result.output.find("60") == std::string::npos) {
        return TestResult(false, "Expected output to contain 60, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Multiple operations test passed", result.output);
}

int main() {
    std::cout << "Running Phase 2 Memory Integration Tests...\n\n";
    
    TestSuite suite("Phase2MemoryIntegration");
    
    suite.addTest("Memory Conversion", testMemoryConversion);
    suite.addTest("i32 Store/Load", testI32StoreLoad);
    suite.addTest("i64 Store/Load", testI64StoreLoad);
    suite.addTest("f32 Store/Load", testF32StoreLoad);
    suite.addTest("f64 Store/Load", testF64StoreLoad);
    suite.addTest("Offset Store/Load", testOffsetStoreLoad);
    suite.addTest("Multiple Operations", testMultipleOperations);
    
    auto results = suite.runAllTests();
    
    // Count passed/failed tests
    int passed = 0;
    int failed = 0;
    
    for (const auto& result : results) {
        if (result.second.success) {
            passed++;
        } else {
            failed++;
        }
    }
    
    std::cout << "\n=== Phase 2 Memory Integration Test Results ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n\n";
    
    if (failed > 0) {
        std::cout << "Some memory integration tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 2 memory integration tests passed!\n";
        return 0;
    }
}