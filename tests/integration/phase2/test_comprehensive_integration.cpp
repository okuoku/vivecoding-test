#include "../../utils/test_helpers.h"
#include <iostream>

using namespace ctransian_test;

// Test combined Phase 2 operations - arithmetic + memory + comparisons
TestResult testCombinedOperations() {
    // Create a simple combined test module in memory
    std::string combinedWat = R"(
;; Combined Phase 2 operations test
(module
  (memory 1)  ;; One page (64KB) of memory
  (export "memory" (memory 0))
  
  ;; Test arithmetic + memory + comparisons together
  (func $test_combined_ops (export "test_combined_ops") (result i32)
    ;; Store some values in memory
    i32.const 10
    i32.const 0
    i32.store           ;; memory[0] = 10
    
    i32.const 20
    i32.const 4
    i32.store           ;; memory[4] = 20
    
    i32.const 5
    i32.const 8
    i32.store           ;; memory[8] = 5
    
    ;; Load values and perform arithmetic
    i32.const 0
    i32.load            ;; load memory[0] (10)
    i32.const 4
    i32.load            ;; load memory[4] (20)
    i32.add             ;; 10 + 20 = 30
    
    i32.const 8
    i32.load            ;; load memory[8] (5)
    i32.mul             ;; 30 * 5 = 150
    
    ;; Perform comparison - should be true (150 > 100)
    i32.const 100
    i32.gt_s            ;; 150 > 100 = 1 (true)
  )
  
  ;; Test more complex operations
  (func $test_complex_chain (export "test_complex_chain") (result i32)
    ;; Memory operations with arithmetic
    i32.const 15
    i32.const 12
    i32.store           ;; memory[12] = 15
    
    i32.const 25
    i32.const 16
    i32.store           ;; memory[16] = 25
    
    ;; Load, add, compare
    i32.const 12
    i32.load            ;; load 15
    i32.const 16
    i32.load            ;; load 25
    i32.add             ;; 15 + 25 = 40
    
    i32.const 5
    i32.mul             ;; 40 * 5 = 200
    i32.const 199
    i32.sub             ;; 200 - 199 = 1
    
    ;; Final comparison
    i32.const 0
    i32.eq              ;; 1 == 0 = 0 (false)
  )
  
  ;; Test floating point operations
  (func $test_float_ops (export "test_float_ops") (result i32)
    ;; Store and load floats
    f32.const 3.14
    f32.const 0
    f32.store           ;; memory[0] = 3.14f
    
    f32.const 2.86
    f32.const 4
    f32.store           ;; memory[4] = 2.86f
    
    ;; Load and add floats
    f32.const 0
    f32.load            ;; load 3.14f
    f32.const 4
    f32.load            ;; load 2.86f
    f32.add             ;; 3.14f + 2.86f = 6.0f
    
    ;; Compare with 6.0f
    f32.const 6.0
    f32.eq              ;; 6.0f == 6.0f = 1 (true)
  )
)";
    
    // Write the combined test to a file
    std::string combinedWatPath = "test_data/phase2/combined_operations.wat";
    std::string combinedWasmPath = "test_data/phase2/combined_operations.wasm";
    
    if (!writeFileContents(combinedWatPath, combinedWat)) {
        return TestResult(false, "Failed to write combined test file", "");
    }
    
    // Convert to WASM
    TestResult result = convertWatToWasm(combinedWatPath, combinedWasmPath);
    if (!result.success) {
        return TestResult(false, "Failed to convert combined test: " + result.message, "");
    }
    
    // Test combined operations
    TestResult execResult = executeWasmFile(combinedWasmPath, "test_combined_ops");
    if (!execResult.success) {
        return TestResult(false, "Failed to execute test_combined_ops: " + execResult.message, execResult.output);
    }
    
    // Should return 1 (true) - 150 > 100
    if (execResult.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + execResult.output, execResult.output);
    }
    
    // Test complex chain
    TestResult complexResult = executeWasmFile(combinedWasmPath, "test_complex_chain");
    if (!complexResult.success) {
        return TestResult(false, "Failed to execute test_complex_chain: " + complexResult.message, complexResult.output);
    }
    
    // Should return 0 (false) - 1 != 0
    if (complexResult.output.find("0") == std::string::npos) {
        return TestResult(false, "Expected output to contain 0 (false), got: " + complexResult.output, complexResult.output);
    }
    
    // Test float operations
    TestResult floatResult = executeWasmFile(combinedWasmPath, "test_float_ops");
    if (!floatResult.success) {
        return TestResult(false, "Failed to execute test_float_ops: " + floatResult.message, floatResult.output);
    }
    
    // Should return 1 (true) - 6.0f == 6.0f
    if (floatResult.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + floatResult.output, floatResult.output);
    }
    
    return TestResult(true, "Combined operations test passed", execResult.output);
}

// Test arithmetic with conditional logic
TestResult testConditionalArithmetic() {
    std::string conditionalWat = R"(
;; Conditional arithmetic test
(module
  (memory 1)
  (export "memory" (memory 0))
  
  (func $test_conditional_math (export "test_conditional_math") (result i32)
    ;; Store two values
    i32.const 10
    i32.const 0
    i32.store
    
    i32.const 20
    i32.const 4
    i32.store
    
    ;; Load and compare
    i32.const 0
    i32.load
    i32.const 4
    i32.load
    i32.lt_s             ;; 10 < 20 = 1 (true)
    
    ;; Use the result in a select operation
    i32.const 100
    i32.const 200
    select               ;; true ? 100 : 200 = 100
  )
)";
    
    std::string conditionalWatPath = "test_data/phase2/conditional_arithmetic.wat";
    std::string conditionalWasmPath = "test_data/phase2/conditional_arithmetic.wasm";
    
    if (!writeFileContents(conditionalWatPath, conditionalWat)) {
        return TestResult(false, "Failed to write conditional test file", "");
    }
    
    TestResult result = convertWatToWasm(conditionalWatPath, conditionalWasmPath);
    if (!result.success) {
        return TestResult(false, "Failed to convert conditional test: " + result.message, "");
    }
    
    TestResult execResult = executeWasmFile(conditionalWasmPath, "test_conditional_math");
    if (!execResult.success) {
        return TestResult(false, "Failed to execute conditional test: " + execResult.message, execResult.output);
    }
    
    // Should return 100 (since 10 < 20 is true)
    if (execResult.output.find("100") == std::string::npos) {
        return TestResult(false, "Expected output to contain 100, got: " + execResult.output, execResult.output);
    }
    
    return TestResult(true, "Conditional arithmetic test passed", execResult.output);
}

// Test mixed integer and float operations
TestResult testMixedIntFloatOps() {
    std::string mixedWat = R"(
;; Mixed integer and float operations test
(module
  (memory 1)
  (export "memory" (memory 0))
  
  (func $test_mixed_ops (export "test_mixed_ops") (result i32)
    ;; Store integers and floats at different memory locations
    i32.const 42
    i32.const 0
    i32.store
    
    f32.const 3.14
    f32.const 4
    f32.store
    
    i64.const 1000000
    i32.const 8
    i64.store
    
    ;; Perform integer arithmetic
    i32.const 0
    i32.load
    i32.const 8
    i32.add             ;; 42 + 8 = 50
    
    ;; Perform float arithmetic (separate)
    f32.const 4
    f32.load            ;; load 3.14
    f32.const 2.0
    f32.add             ;; 3.14 + 2.0 = 5.14
    f32.const 5.14
    f32.eq              ;; 5.14 == 5.14 = 1 (true)
    
    ;; Combine results
    i32.add             ;; 50 + 1 = 51
  )
)";
    
    std::string mixedWatPath = "test_data/phase2/mixed_operations.wat";
    std::string mixedWasmPath = "test_data/phase2/mixed_operations.wasm";
    
    if (!writeFileContents(mixedWatPath, mixedWat)) {
        return TestResult(false, "Failed to write mixed operations test file", "");
    }
    
    TestResult result = convertWatToWasm(mixedWatPath, mixedWasmPath);
    if (!result.success) {
        return TestResult(false, "Failed to convert mixed operations test: " + result.message, "");
    }
    
    TestResult execResult = executeWasmFile(mixedWasmPath, "test_mixed_ops");
    if (!execResult.success) {
        return TestResult(false, "Failed to execute mixed operations test: " + execResult.message, execResult.output);
    }
    
    // Should return 51 (50 + 1)
    if (execResult.output.find("51") == std::string::npos) {
        return TestResult(false, "Expected output to contain 51, got: " + execResult.output, execResult.output);
    }
    
    return TestResult(true, "Mixed operations test passed", execResult.output);
}

int main() {
    std::cout << "Running Phase 2 Comprehensive Integration Tests...\n\n";
    
    TestSuite suite("Phase2ComprehensiveIntegration");
    
    suite.addTest("Combined Operations", testCombinedOperations);
    suite.addTest("Conditional Arithmetic", testConditionalArithmetic);
    suite.addTest("Mixed Int/Float Operations", testMixedIntFloatOps);
    
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
    
    std::cout << "\n=== Phase 2 Comprehensive Integration Test Results ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n\n";
    
    if (failed > 0) {
        std::cout << "Some comprehensive integration tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 2 comprehensive integration tests passed!\n";
        return 0;
    }
}