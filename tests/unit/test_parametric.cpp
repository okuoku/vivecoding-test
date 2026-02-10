#include "../utils/test_helpers.h"
#include <iostream>

using namespace ctransian_test;

// Test conversion of parametric instruction test file
TestResult testParametricConversion() {
    std::string watFile = "test_data/phase1/parametric/parametric_tests.wat";
    std::string wasmFile = "test_data/phase1/parametric/parametric_tests.wasm";
    
    TestResult result = convertWatToWasm(watFile, wasmFile);
    if (!result.success) {
        return TestResult(false, "Failed to convert parametric test file: " + result.message, "");
    }
    
    return TestResult(true, "Parametric test file converted successfully", "");
}

// Test drop instruction execution
TestResult testDropInstruction() {
    TestResult result = executeWasmFile("test_data/phase1/parametric/parametric_tests.wasm", "test_drop_single");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_drop_single: " + result.message, result.output);
    }
    
    // Should return 456 (the value that wasn't dropped)
    if (result.output.find("456") == std::string::npos) {
        return TestResult(false, "Expected output to contain 456, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Drop instruction test passed", result.output);
}

// Test select instruction with true condition
TestResult testSelectInstructionTrue() {
    TestResult result = executeWasmFile("test_data/phase1/parametric/parametric_tests.wasm", "test_select_true");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_select_true: " + result.message, result.output);
    }
    
    // Should return 42 (true value)
    if (result.output.find("42") == std::string::npos) {
        return TestResult(false, "Expected output to contain 42, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Select instruction (true condition) test passed", result.output);
}

// Test select instruction with false condition
TestResult testSelectInstructionFalse() {
    TestResult result = executeWasmFile("test_data/phase1/parametric/parametric_tests.wasm", "test_select_false");
    if (!result.success) {
        return TestResult(false, "Failed to execute test_select_false: " + result.message, result.output);
    }
    
    // Should return 24 (false value)
    if (result.output.find("24") == std::string::npos) {
        return TestResult(false, "Expected output to contain 24, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Select instruction (false condition) test passed", result.output);
}

int main() {
    std::cout << "Running Phase 1 Parametric Instruction Tests...\n\n";
    
    TestSuite suite("Phase1Parametric");
    
    suite.addTest("Parametric Conversion", testParametricConversion);
    suite.addTest("Drop Instruction", testDropInstruction);
    suite.addTest("Select True", testSelectInstructionTrue);
    suite.addTest("Select False", testSelectInstructionFalse);
    
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
    
    std::cout << "\n=== Phase 1 Parametric Test Results ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n\n";
    
    if (failed > 0) {
        std::cout << "Some parametric tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 1 parametric tests passed!\n";
        return 0;
    }
}