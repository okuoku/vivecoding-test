#include "../../utils/test_helpers.h"
#include <iostream>

using namespace ctransian_test;

// Test conversion of comparison operations test file
TestResult testComparisonConversion() {
    std::string watFile = "test_data/phase2/comparisons/comparison_operations.wat";
    std::string wasmFile = "test_data/phase2/comparisons/comparison_operations.wasm";
    
    TestResult result = convertWatToWasm(watFile, wasmFile);
    if (!result.success) {
        return TestResult(false, "Failed to convert comparison operations test file: " + result.message, "");
    }
    
    return TestResult(true, "Comparison operations test file converted successfully", "");
}

// Test i32.eq true condition
TestResult testI32EqTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_eq_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_eq_true: " + result.message, result.output);
    }
    
    // Should return 1 (true)
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.eq true test passed", result.output);
}

// Test i32.eq false condition
TestResult testI32EqFalse() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_eq_false");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_eq_false: " + result.message, result.output);
    }
    
    // Should return 0 (false)
    if (result.output.find("0") == std::string::npos) {
        return TestResult(false, "Expected output to contain 0 (false), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.eq false test passed", result.output);
}

// Test i32.ne true condition
TestResult testI32NeTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_ne_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_ne_true: " + result.message, result.output);
    }
    
    // Should return 1 (true)
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.ne true test passed", result.output);
}

// Test i32.ne false condition
TestResult testI32NeFalse() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_ne_false");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_ne_false: " + result.message, result.output);
    }
    
    // Should return 0 (false)
    if (result.output.find("0") == std::string::npos) {
        return TestResult(false, "Expected output to contain 0 (false), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.ne false test passed", result.output);
}

// Test i32.lt_s true condition
TestResult testI32LtSTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_lt_s_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_lt_s_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 10 < 20
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.lt_s true test passed", result.output);
}

// Test i32.lt_s false condition
TestResult testI32LtSFalse() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_lt_s_false");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_lt_s_false: " + result.message, result.output);
    }
    
    // Should return 0 (false) - 30 < 20 is false
    if (result.output.find("0") == std::string::npos) {
        return TestResult(false, "Expected output to contain 0 (false), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.lt_s false test passed", result.output);
}

// Test i32.gt_s true condition
TestResult testI32GtSTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_gt_s_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_gt_s_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 30 > 20
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.gt_s true test passed", result.output);
}

// Test i32.le_s true condition (equality)
TestResult testI32LeSTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_le_s_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_le_s_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 20 <= 20
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.le_s true test passed", result.output);
}

// Test i32.ge_s true condition (equality)
TestResult testI32GeSTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_ge_s_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_ge_s_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 20 >= 20
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.ge_s true test passed", result.output);
}

// Test i32.lt_u true condition
TestResult testI32LtUTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i32_lt_u_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i32_lt_u_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 10 < 20 (unsigned)
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i32.lt_u true test passed", result.output);
}

// Test i64.eq true condition
TestResult testI64EqTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_i64_eq_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_i64_eq_true: " + result.message, result.output);
    }
    
    // Should return 1 (true)
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "i64.eq true test passed", result.output);
}

// Test f32.eq true condition
TestResult testF32EqTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_f32_eq_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f32_eq_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 1.5 == 1.5
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "f32.eq true test passed", result.output);
}

// Test f32.eq false condition
TestResult testF32EqFalse() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_f32_eq_false");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f32_eq_false: " + result.message, result.output);
    }
    
    // Should return 0 (false) - 1.5 != 2.5
    if (result.output.find("0") == std::string::npos) {
        return TestResult(false, "Expected output to contain 0 (false), got: " + result.output, result.output);
    }
    
    return TestResult(true, "f32.eq false test passed", result.output);
}

// Test f32.lt true condition
TestResult testF32LtTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_f32_lt_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f32_lt_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 1.5 < 2.5
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "f32.lt true test passed", result.output);
}

// Test f64.eq true condition
TestResult testF64EqTrue() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_f64_eq_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_f64_eq_true: " + result.message, result.output);
    }
    
    // Should return 1 (true) - 3.141592653589793 == 3.141592653589793
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "f64.eq true test passed", result.output);
}

// Test complex comparison chain
TestResult testComparisonChain() {
    TestResult result = executeWasmFile("test_data/phase2/comparisons/comparison_operations.wasm", "test_comparison_chain");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_comparison_chain: " + result.message, result.output);
    }
    
    // Should return 1 (true) - all conditions are true
    if (result.output.find("1") == std::string::npos) {
        return TestResult(false, "Expected output to contain 1 (true), got: " + result.output, result.output);
    }
    
    return TestResult(true, "Comparison chain test passed", result.output);
}

int main() {
    std::cout << "Running Phase 2 Comparison Integration Tests...\n\n";
    
    TestSuite suite("Phase2ComparisonIntegration");
    
    suite.addTest("Comparison Conversion", testComparisonConversion);
    suite.addTest("i32 Eq True", testI32EqTrue);
    suite.addTest("i32 Eq False", testI32EqFalse);
    suite.addTest("i32 Ne True", testI32NeTrue);
    suite.addTest("i32 Ne False", testI32NeFalse);
    suite.addTest("i32 Lt S True", testI32LtSTrue);
    suite.addTest("i32 Lt S False", testI32LtSFalse);
    suite.addTest("i32 Gt S True", testI32GtSTrue);
    suite.addTest("i32 Le S True", testI32LeSTrue);
    suite.addTest("i32 Ge S True", testI32GeSTrue);
    suite.addTest("i32 Lt U True", testI32LtUTrue);
    suite.addTest("i64 Eq True", testI64EqTrue);
    suite.addTest("f32 Eq True", testF32EqTrue);
    suite.addTest("f32 Eq False", testF32EqFalse);
    suite.addTest("f32 Lt True", testF32LtTrue);
    suite.addTest("f64 Eq True", testF64EqTrue);
    suite.addTest("Comparison Chain", testComparisonChain);
    
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
    
    std::cout << "\n=== Phase 2 Comparison Integration Test Results ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n\n";
    
    if (failed > 0) {
        std::cout << "Some comparison integration tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 2 comparison integration tests passed!\n";
        return 0;
    }
}