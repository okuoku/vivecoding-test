#ifndef CTRANIAN_TRANSLATOR_H
#define CTRANIAN_TRANSLATOR_H

#include <binaryen-c.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "ctransian/public.h"

namespace ctransian {

// Forward declarations
class TypeMapper;
class InstructionVisitor;
class CodeGenerator;

/**
 * Main translation context and engine
 */
class Translator {
public:
    explicit Translator(const ctransian_config_t* config);
    ~Translator();

    // Main translation methods
    ctransian_error_t translateBinary(
        const uint8_t* wasm_data,
        size_t wasm_size,
        ctransian_result_t* result
    );

    ctransian_error_t translateText(
        const char* wat_text,
        ctransian_result_t* result
    );

    // Analysis methods
    ctransian_error_t analyzeBinary(
        const uint8_t* wasm_data,
        size_t wasm_size,
        ctransian_module_info_t* info
    );

    ctransian_error_t analyzeText(
        const char* wat_text,
        ctransian_module_info_t* info
    );

    // Validation methods
    ctransian_error_t validateBinary(
        const uint8_t* wasm_data,
        size_t wasm_size
    );

    ctransian_error_t validateText(const char* wat_text);

    // Configuration
    const ctransian_config_t* getConfig() const { return config_; }

private:
    // Core translation pipeline
    ctransian_error_t parseModule(const uint8_t* data, size_t size);
    ctransian_error_t parseTextModule(const char* text);
    ctransian_error_t optimizeModule();
    ctransian_error_t analyzeModule();
    ctransian_error_t generateCode(ctransian_result_t* result);

    // Helper methods
    ctransian_error_t validateConfiguration();
    void reset();
    std::string generateErrorMessage(BinaryenExpressionRef expr);
    std::string getDiagnosticInfo();

    // Configuration and state
    const ctransian_config_t* config_;
    
    // Binaryen module
    BinaryenModuleRef module_;
    
    // Component objects
    std::unique_ptr<TypeMapper> type_mapper_;
    std::unique_ptr<InstructionVisitor> instruction_visitor_;
    std::unique_ptr<CodeGenerator> code_generator_;
    
    // Analysis data
    ctransian_module_info_t module_info_;
    bool module_analyzed_;
    
    // Translation state
    std::vector<std::string> warnings_;
    std::string last_error_;
};

/**
 * Translation utilities
 */
namespace utils {
    // String utilities
    std::string escapeCIdentifier(const std::string& name);
    std::string sanitizeFunctionName(const std::string& name);
    std::string generateUniqueName(const std::string& prefix);
    
    // Type utilities
    bool isWasmIntegerType(BinaryenType type);
    bool isWasmFloatType(BinaryenType type);
    bool isWasmVectorType(BinaryenType type);
    bool isWasmReferenceType(BinaryenType type);
    
    // Validation utilities
    bool isValidCIdentifier(const std::string& name);
    bool isSupportedWasmFeature(BinaryenExpressionRef expr);
}

} // namespace ctransian

#endif // CTRANIAN_TRANSLATOR_H