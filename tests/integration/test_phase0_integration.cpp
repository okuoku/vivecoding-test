#include "../utils/test_helpers.h"
#include <iostream>
#include <algorithm>

using namespace ctransian_test;

// Test conversion of Phase 0 constant WAT files to WASM
TestResult testPhase0ConstantsConversion() {
    std::vector<std::string> testFiles = {
        "test_data/phase0/constants/simple_constants.wat",
        "test_data/phase0/constants/special_floats.wat", 
        "test_data/phase0/constants/extreme_values.wat"
    };
    
    for (const auto& watFile : testFiles) {
        std::string wasmFile = watFile;
        wasmFile.replace(wasmFile.find(".wat"), 4, ".wasm");
        
        TestResult result = convertWatToWasm(watFile, wasmFile);
        if (!result.success) {
            return TestResult(false, "Failed to convert " + watFile + ": " + result.message, "");
        }
        
        // Validate generated WASM file
        TestResult validation = validateWasmFile(wasmFile, watFile);
        if (!validation.success) {
            return TestResult(false, "Failed to validate " + wasmFile + ": " + validation.message, "");
        }
    }
    
    return TestResult(true, "All Phase 0 constant files converted and validated successfully", "");
}

// Test validation of simple constant functions
TestResult testPhase0ConstantsValidation() {
    // Test simple constants - use generated WASM file in build directory
    TestResult result = executeWasmFile("test_data/phase0/constants/simple_constants.wasm", "test_i32");
    if (!result.success) {
        return TestResult(false, "Failed to validate test_i32: " + result.message, result.output);
    }
    
    // The output should contain to function
    if (result.output.find("(export \"test_i32\"") == std::string::npos) {
        return TestResult(false, "Expected output to contain test_i32 function, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Phase 0 constant validation test passed", result.output);
}

// Test validation of special float values
TestResult testPhase0SpecialFloats() {
    // Test NaN value
    TestResult result = executeWasmFile("test_data/phase0/constants/special_floats.wasm", "test_f32_nan");
    if (!result.success) {
        return TestResult(false, "Failed to validate test_f32_nan: " + result.message, result.output);
    }
    
    // The output should contain to function
    if (result.output.find("(export \"test_f32_nan\"") == std::string::npos) {
        return TestResult(false, "Expected output to contain test_f32_nan function, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Phase 0 special float validation test passed", result.output);
}

// Test validation of extreme integer values
TestResult testPhase0ExtremeValues() {
    // Test i32 maximum value
    TestResult result = executeWasmFile("test_data/phase0/constants/extreme_values.wasm", "test_i32_max");
    if (!result.success) {
        return TestResult(false, "Failed to validate test_i32_max: " + result.message, result.output);
    }
    
    // The output should contain to function
    if (result.output.find("(export \"test_i32_max\"") == std::string::npos) {
        return TestResult(false, "Expected output to contain test_i32_max function, got: " + result.output, result.output);
    }
    
    return TestResult(true, "Phase 0 extreme values validation test passed", result.output);
}

// Test that all generated WASM files are valid
TestResult testPhase0WasmFileValidity() {
    std::vector<std::string> wasmFiles = {
        "test_data/phase0/constants/simple_constants.wasm",
        "test_data/phase0/constants/special_floats.wasm",
        "test_data/phase0/constants/extreme_values.wasm"
    };
    
    for (const auto& wasmFile : wasmFiles) {
        std::ifstream file(wasmFile);
        if (!file.good()) {
            return TestResult(false, "WASM file does not exist: " + wasmFile, "");
        }
        file.close();
        
        // Check file size (should be non-zero)
        std::ifstream sizeCheck(wasmFile, std::ios::binary | std::ios::ate);
        if (!sizeCheck.good()) {
            return TestResult(false, "Cannot read WASM file: " + wasmFile, "");
        }
        
        std::streamsize size = sizeCheck.tellg();
        sizeCheck.close();
        
        if (size <= 0) {
            return TestResult(false, "WASM file is empty: " + wasmFile, "");
        }
        
        // Check magic number (0x00 0x61 0x73 0x6D)
        std::ifstream magicCheck(wasmFile, std::ios::binary);
        char magic[4];
        magicCheck.read(magic, 4);
        magicCheck.close();
        
        if (magic[0] != 0x00 || magic[1] != 0x61 || magic[2] != 0x73 || magic[3] != 0x6D) {
            return TestResult(false, "WASM file has invalid magic number: " + wasmFile, "");
        }
    }
    
    return TestResult(true, "All Phase 0 WASM files are valid", "");
}

int main() {
    std::cout << "Running Phase 0 Integration Tests...\n\n";
    
    TestSuite suite("Phase0Integration");
    
    suite.addTest("Constants Conversion", testPhase0ConstantsConversion);
    suite.addTest("Constants Validation", testPhase0ConstantsValidation);
    suite.addTest("Special Floats", testPhase0SpecialFloats);
    suite.addTest("Extreme Values", testPhase0ExtremeValues);
    suite.addTest("WASM File Validity", testPhase0WasmFileValidity);
    
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
    
    std::cout << "\n=== Phase 0 Integration Test Results ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n\n";
    
    if (failed > 0) {
        std::cout << "Some tests failed. See details above.\n";
        return 1;
    } else {
        std::cout << "All Phase 0 integration tests passed!\n";
        return 0;
    }
}