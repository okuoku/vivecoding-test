#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <cstdio>

// Test helper utilities for ctransian testing

namespace ctransian_test {

// Structure to hold test result
struct TestResult {
    bool success;
    std::string message;
    std::string output;
    
    TestResult(bool s = false, const std::string& msg = "", const std::string& out = "")
        : success(s), message(msg), output(out) {}
};

// Convert .wat file to .wasm using Binaryen wasm-as
TestResult convertWatToWasm(const std::string& watPath, const std::string& wasmPath);

// Validate .wasm file by converting back to .wat and comparing
TestResult validateWasmFile(const std::string& wasmPath, const std::string& originalWatPath);

// Run a .wasm file with wasm-shell to test execution
TestResult executeWasmFile(const std::string& wasmPath, const std::string& functionName);

// Read file contents
std::string readFileContents(const std::string& filePath);

// Write file contents
bool writeFileContents(const std::string& filePath, const std::string& contents);

// Compare two files
bool compareFiles(const std::string& file1Path, const std::string& file2Path);

// Get the path to built Binaryen tools
std::string getBinaryenToolPath(const std::string& toolName);

// Create a temporary directory for test outputs
std::string createTempDirectory();

// Clean up temporary files
void cleanupTempFiles(const std::vector<std::string>& files);

// Assert macros for testing
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            return TestResult(false, "Assertion failed: " #condition, ""); \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            return TestResult(false, "Assertion failed: " #condition " should be false", ""); \
        } \
    } while(0)

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            return TestResult(false, "Assertion failed: expected " + std::to_string(expected) + " but got " + std::to_string(actual), ""); \
        } \
    } while(0)

#define ASSERT_STR_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            return TestResult(false, "Assertion failed: expected '" + std::string(expected) + "' but got '" + std::string(actual) + "'", ""); \
        } \
    } while(0)

// Test function type
using TestFunction = std::function<TestResult()>;

// Test suite class
class TestSuite {
public:
    TestSuite(const std::string& name) : suiteName(name) {}
    
    void addTest(const std::string& testName, TestFunction testFunc) {
        tests.push_back({testName, testFunc});
    }
    
    std::vector<std::pair<std::string, TestResult>> runAllTests() {
        std::vector<std::pair<std::string, TestResult>> results;
        
        for (const auto& test : tests) {
            std::cout << "Running test: " << test.first << "... ";
            TestResult result = test.second();
            
            if (result.success) {
                std::cout << "PASSED\n";
            } else {
                std::cout << "FAILED: " << result.message << "\n";
            }
            
            results.push_back({test.first, result});
        }
        
        return results;
    }
    
private:
    std::string suiteName;
    std::vector<std::pair<std::string, TestFunction>> tests;
};

} // namespace ctransian_test

#endif // TEST_HELPERS_H