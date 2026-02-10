#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

int run_test_command(const std::string& test_name, const std::string& command) {
    std::cout << "Running " << test_name << "...\n";
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "✓ " << test_name << " PASSED\n";
    } else {
        std::cout << "✗ " << test_name << " FAILED (exit code: " << result << ")\n";
    }
    return result;
}

int main() {
    std::cout << "Running all Phase 2 tests for ctransian...\n";
    std::cout << "==========================================\n\n";
    
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    
    // List of all Phase 2 test executables
    std::vector<std::pair<std::string, std::string>> tests = {
        {"Unit Tests", ""},
        {"Memory Operations Unit Tests", "./tests/unit/phase2/test_memory_operations"},
        {"Arithmetic Operations Unit Tests", "./tests/unit/phase2/test_arithmetic_operations"},
        {"Comparison Operations Unit Tests", "./tests/unit/phase2/test_comparison_operations"},
        {"Integration Tests", ""},
        {"Memory Integration Tests", "./tests/integration/phase2/test_memory_integration"},
        {"Arithmetic Integration Tests", "./tests/integration/phase2/test_arithmetic_integration"},
        {"Comparison Integration Tests", "./tests/integration/phase2/test_comparison_integration"},
        {"Comprehensive Integration Tests", "./tests/integration/phase2/test_comprehensive_integration"}
    };
    
    for (const auto& test : tests) {
        if (test.second.empty()) {
            // This is a section header
            std::cout << "\n=== " << test.first << " ===\n";
            continue;
        }
        
        total_tests++;
        int result = run_test_command(test.first, test.second);
        if (result == 0) {
            passed_tests++;
        } else {
            failed_tests++;
        }
        std::cout << "\n";
    }
    
    // Final summary
    std::cout << "==========================================\n";
    std::cout << "Phase 2 Test Summary:\n";
    std::cout << "Total tests: " << total_tests << "\n";
    std::cout << "Passed: " << passed_tests << "\n";
    std::cout << "Failed: " << failed_tests << "\n\n";
    
    if (failed_tests > 0) {
        std::cout << "Some Phase 2 tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 2 tests passed successfully!\n";
        std::cout << "\nPhase 2 Implementation Status:\n";
        std::cout << "✅ Memory Operations (load, store, memory.size, memory.grow)\n";
        std::cout << "✅ Basic Arithmetic (add, sub, mul for i32, i64, f32, f64)\n";
        std::cout << "✅ Comparisons (eq, ne, lt, gt, le, ge for all types)\n";
        std::cout << "✅ Integration and Comprehensive Testing\n";
        std::cout << "\nReady to proceed to Phase 3: Advanced Operations (bitwise, conversions, division)!\n";
        return 0;
    }
}