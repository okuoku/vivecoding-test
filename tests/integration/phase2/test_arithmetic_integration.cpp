#include "../../utils/test_helpers.h"
#include <iostream>

using namespace ctransian_test;

// Test conversion of arithmetic operations test file
TestResult testArithmeticConversion() {
    std::string watFile = "test_data/phase2/arithmetic/arithmetic_operations.wat";
    std::string wasmFile = "test_data/phase2/arithmetic/arithmetic_operations.wasm";
    
    TestResult result = convertWatToWasm(watFile, wasmFile);
    if (!result.success) {
        return TestResult(false, "Failed to convert arithmetic operations test file: " + result.message, "");
    }
    
    return TestResult(true, "Arithmetic operations test file converted successfully", "");
}

// Test i32.add operation
TestResult testI32Add() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i32_add");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_add: " + result.message, result.output);
    }
    
    // Should return 42 (10 + 32)
    if (result.output.find("42") == std::string::npos) {
        return TestResult(false, "Expected output to contain 42, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.add test passed", result.output);
}

// Test i32.sub operation
TestResult testI32Sub() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i32_sub");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_sub: " + result.message, result.output);
    }
    
    // Should return 75 (100 - 25)
    if (result.output.find("75") == std::string::npos) {
        return TestResult(false, "Expected output to contain 75, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.sub test passed", result.output);
}

// Test i32.mul operation
TestResult testI32Mul() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i32_mul");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_mul: " + result.message, result.output);
    }
    
    // Should return 96 (12 * 8)
    if (result.output.find("96") == std::string::npos) {
        return TestResult(false, "Expected output to contain 96, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.mul test passed", result.output);
}

// Test i32 complex arithmetic chain
TestResult testI32Complex() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i32_complex");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_complex: " + result.message, result.output);
    }
    
    // Should return 30 ((5 + 10) * 3 - 15 = 30)
    if (result.output.find("30") == std::string::npos) {
        return TestResult(false, "Expected output to contain 30, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32 complex arithmetic test passed", result.output);
}

// Test i64.add operation
TestResult testI64Add() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i64_add");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i64_add: " + result.message, result.output);
    }
    
    // Should return 30000000000 (10000000000 + 20000000000)
    if (result.output.find("30000000000") == std::string::npos) {
        return TestResult(false, "Expected output to contain 30000000000, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i64.add test passed", result.output);
}

// Test i64.sub operation
TestResult testI64Sub() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i64_sub");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i64_sub: " + result.message, result.output);
    }
    
    // Should return 25000000000 (50000000000 - 25000000000)
    if (result.output.find("25000000000") == std::string::npos) {
        return TestResult(false, "Expected output to contain 25000000000, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i64.sub test passed", result.output);
}

// Test i64.mul operation
TestResult testI64Mul() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_i64_mul");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i64_mul: " + result.message, result.output);
    }
    
    // Should return 1000000000 (1000000 * 1000)
    if (result.output.find("1000000000") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1000000000, got: " + result.output, result.output);
    }
    
    return TestResult(true, "i64.mul test passed", result.output);
}

// Test f32.add operation
TestResult testF32Add() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_f32_add");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f32_add: " + result.message, result.output);
    }
    
    // Should return 4.0 (1.5 + 2.5)
    if (result.output.find("4") == std::string::npos) {
        return TestResult(false, "Expected output to contain 4, got: " + result.output, result.output);
    }
    
    return TestResult(true, "f32.add test passed", result.output);
}

// Test f64.add operation
TestResult testF64Add() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_f64_add");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f64_add: " + result.message, result.output);
    }
    
    // Should return approximately 11.11111110 (1.23456789 + 9.87654321)
    if (result.output.find("11") == std::string::npos) {
        return TestResult(false, "Expected output to contain approximately 11, got: " + result.output, result.output);
    }
    
    return TestResult(true, "f64.add test passed", result.output);
}

// Test mixed arithmetic chain
TestResult testMixedArithmetic() {
    TestResult result = executeWasmFile("test_data/phase2/arithmetic/arithmetic_operations.wasm", "test_mixed_arithmetic");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_mixed_arithmetic: " + result.message, result.output);
    }
    
    // Should return 15 ((2 + 3) * 4 - 10 + 5 = 15)
    if (result.output.find("15") == std::string::npos) {
        return TestResult(false, "Expected output to contain 15, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Mixed arithmetic test passed", result.output);
}

int main() {
    std::cout << "Running Phase 2 Arithmetic Integration Tests...\n\n";
    
    TestSuite suite("Phase2ArithmeticIntegration");
    
    suite.addTest("Arithmetic Conversion", testArithmeticConversion);
    suite.addTest("i32 Add", testI32Add);
    suite.addTest("i32 Sub", testI32Sub);
    suite.addTest("i32 Mul", testI32Mul);
    suite.addTest("i32 Complex", testI32Complex);
    suite.addTest("i64 Add", testI64Add);
    suite.addTest("i64 Sub", testI64Sub);
    suite.addTest("i64 Mul", testI64Mul);
    suite.addTest("f32 Add", testF32Add);
    suite.addTest("f64 Add", testF64Add);
    suite.addTest("Mixed Arithmetic", testMixedArithmetic);
    
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
    
    std::cout << "\n=== Phase 2 Arithmetic Integration Test Results ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n\n";
    
    if (failed > 0) {
        std::cout << "Some arithmetic integration tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 2 arithmetic integration tests passed!\n";
        return 0;
    }
}