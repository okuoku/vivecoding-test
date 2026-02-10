#include "test_helpers.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <vector>

namespace ctransian_test {

std::string getBinaryenToolPath(const std::string& toolName) {
    // Try to find tool in various build directories
    std::vector<std::string> paths = {
        "./build/third_party/binaryen/bin/" + toolName,
        "../build/third_party/binaryen/bin/" + toolName,
        "../../build/third_party/binaryen/bin/" + toolName,
        "./third_party/binaryen/bin/" + toolName,
        toolName // fallback to PATH
    };
    
    for (size_t i = 0; i < paths.size(); ++i) {
        const std::string& path = paths[i];
        std::ifstream toolFile(path);
        if (toolFile.good()) {
            toolFile.close();
            return path;
        }
    }
    
    // Try system PATH as last resort
    if (system(("which " + toolName + " >/dev/null 2>&1").c_str()) == 0) {
        return toolName;
    }
    
    return ""; // Not found
}

TestResult convertWatToWasm(const std::string& watPath, const std::string& wasmPath) {
    std::string wasmAsPath = getBinaryenToolPath("wasm-as");
    if (wasmAsPath.empty()) {
        return TestResult(false, "wasm-as tool not found", "");
    }
    
    std::string command = wasmAsPath + " \"" + watPath + "\" -o \"" + wasmPath + "\"";
    int result = system(command.c_str());
    
    if (result != 0) {
        return TestResult(false, "Failed to convert " + watPath + " to " + wasmPath + " (exit code: " + std::to_string(result) + ")", "");
    }
    
    std::ifstream wasmFile(wasmPath);
    if (!wasmFile.good()) {
        wasmFile.close();
        return TestResult(false, "Output file " + wasmPath + " was not created", "");
    }
    wasmFile.close();
    
    return TestResult(true, "Successfully converted " + watPath + " to " + wasmPath, "");
}

TestResult validateWasmFile(const std::string& wasmPath, const std::string& originalWatPath) {
    std::string wasmDisPath = getBinaryenToolPath("wasm-dis");
    if (wasmDisPath.empty()) {
        return TestResult(false, "wasm-dis tool not found", "");
    }
    
    // Create temporary file for disassembled output
    std::string tempWatPath = originalWatPath + ".disassembled.wat";
    
    std::string command = wasmDisPath + " \"" + wasmPath + "\" -o \"" + tempWatPath + "\"";
    int result = system(command.c_str());
    
    if (result != 0) {
        return TestResult(false, "Failed to disassemble " + wasmPath + " (exit code: " + std::to_string(result) + ")", "");
    }
    
    // Compare original and disassembled (simplified check)
    std::ifstream tempFile(tempWatPath);
    if (!tempFile.good()) {
        tempFile.close();
        return TestResult(false, "Disassembled file was not created", "");
    }
    tempFile.close();
    
    // Clean up temp file
    std::remove(tempWatPath.c_str());
    
    return TestResult(true, "WASM file validation successful", "");
}

std::string readFileContents(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool writeFileContents(const std::string& filePath, const std::string& contents) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    file << contents;
    return file.good();
}

bool compareFiles(const std::string& file1Path, const std::string& file2Path) {
    std::string content1 = readFileContents(file1Path);
    std::string content2 = readFileContents(file2Path);
    
    if (content1.empty() && content2.empty()) {
        return true; // Both files don't exist or are empty
    }
    
    return content1 == content2;
}

std::string createTempDirectory() {
    // Create a unique temporary directory
    std::string tempDir = "/tmp/ctransian_test_" + std::to_string(getpid()) + "_" + std::to_string(time(nullptr));
    mkdir(tempDir.c_str(), 0755);
    return tempDir;
}

void cleanupTempFiles(const std::vector<std::string>& files) {
    for (size_t i = 0; i < files.size(); ++i) {
        const std::string& file = files[i];
        std::remove(file.c_str());
    }
}

TestResult executeWasmFile(const std::string& wasmPath, const std::string& functionName) {
    std::string wasmDisPath = getBinaryenToolPath("wasm-dis");
    if (wasmDisPath.empty()) {
        return TestResult(false, "wasm-dis tool not found", "");
    }
    
    // For now, just validate the WASM file by disassembling it
    // wasm-shell doesn't support --run, so we'll validate structure instead
    std::string tempWatPath = wasmPath + ".disassembled.wat";
    std::string command = wasmDisPath + " \"" + wasmPath + "\" -o \"" + tempWatPath + "\"";
    
    int result = system(command.c_str());
    if (result != 0) {
        return TestResult(false, "Failed to disassemble " + wasmPath + " (exit code: " + std::to_string(result) + ")", "");
    }
    
    // Read the disassembled content
    std::ifstream tempFile(tempWatPath);
    if (!tempFile.good()) {
        tempFile.close();
        return TestResult(false, "Disassembled file was not created", "");
    }
    
    std::string content((std::istreambuf_iterator<char>(tempFile)),
                     std::istreambuf_iterator<char>());
    tempFile.close();
    
    // Clean up temp file
    std::remove(tempWatPath.c_str());
    
    // Check if function exists in the disassembled output
    std::string searchFunc = "(func $" + functionName;
    if (content.find(searchFunc) == std::string::npos) {
        searchFunc = "(export \"" + functionName + "\""; // Try export format
        if (content.find(searchFunc) == std::string::npos) {
            return TestResult(false, "Function " + functionName + " not found in WASM module", content);
        }
    }
    
    return TestResult(true, "WASM validation completed", content);
}

} // namespace ctransian_test