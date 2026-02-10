#include "translator.h"
#include "type_mapper.h"
#include "instruction_visitor.h"
#include "code_generator.h"
#include <binaryen-c.h>
#include <cstring>
#include <memory>

namespace ctransian {

Translator::Translator(const ctransian_config_t* config)
    : config_(config), module_(nullptr), module_analyzed_(false) {
    
    // Validate configuration
    if (validateConfiguration() != CTRANIAN_SUCCESS) {
        throw std::runtime_error("Invalid configuration");
    }
    
    // Initialize component objects
    type_mapper_ = std::make_unique<TypeMapper>(config_);
    instruction_visitor_ = std::make_unique<InstructionVisitor>(type_mapper_.get(), config_);
    code_generator_ = std::make_unique<CodeGenerator>(config_);
    
    // Initialize module info
    std::memset(&module_info_, 0, sizeof(ctransian_module_info_t));
}

Translator::~Translator() {
    reset();
}

ctransian_error_t Translator::translateBinary(
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_result_t* result
) {
    if (!wasm_data || wasm_size == 0 || !result) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    try {
        // Reset state
        reset();
        
        // Parse module
        ctransian_error_t error = parseModule(wasm_data, wasm_size);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Optimize module
        error = optimizeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Analyze module
        error = analyzeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Generate code
        error = generateCode(result);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        return CTRANIAN_SUCCESS;
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_TRANSLATION_FAILED;
    }
}

ctransian_error_t Translator::translateText(
    const char* wat_text,
    ctransian_result_t* result
) {
    if (!wat_text || !result) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    try {
        // Reset state
        reset();
        
        // Parse text module
        ctransian_error_t error = parseTextModule(wat_text);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Optimize module
        error = optimizeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Analyze module
        error = analyzeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Generate code
        error = generateCode(result);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        return CTRANIAN_SUCCESS;
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_TRANSLATION_FAILED;
    }
}

ctransian_error_t Translator::analyzeBinary(
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_module_info_t* info
) {
    if (!wasm_data || wasm_size == 0 || !info) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    try {
        // Reset state
        reset();
        
        // Parse module
        ctransian_error_t error = parseModule(wasm_data, wasm_size);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Analyze module
        error = analyzeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Copy analysis result
        *info = module_info_;
        
        return CTRANIAN_SUCCESS;
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_TRANSLATION_FAILED;
    }
}

ctransian_error_t Translator::analyzeText(
    const char* wat_text,
    ctransian_module_info_t* info
) {
    if (!wat_text || !info) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    try {
        // Reset state
        reset();
        
        // Parse text module
        ctransian_error_t error = parseTextModule(wat_text);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Analyze module
        error = analyzeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Copy analysis result
        *info = module_info_;
        
        return CTRANIAN_SUCCESS;
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_TRANSLATION_FAILED;
    }
}

ctransian_error_t Translator::validateBinary(
    const uint8_t* wasm_data,
    size_t wasm_size
) {
    if (!wasm_data || wasm_size == 0) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    try {
        // Parse module to validate
        reset();
        return parseModule(wasm_data, wasm_size);
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_VALIDATION_FAILED;
    }
}

ctransian_error_t Translator::validateText(const char* wat_text) {
    if (!wat_text) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    try {
        // Parse module to validate
        reset();
        return parseTextModule(wat_text);
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_VALIDATION_FAILED;
    }
}

ctransian_error_t Translator::parseModule(const uint8_t* data, size_t size) {
    if (!data || size == 0) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Create Binaryen module from binary data
    module_ = BinaryenModuleRead(data, size);
    if (!module_) {
        return CTRANIAN_ERROR_PARSE_FAILED;
    }
    
    return CTRANIAN_SUCCESS;
}

ctransian_error_t Translator::parseTextModule(const char* text) {
    if (!text) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Create Binaryen module from text
    module_ = BinaryenModuleParse(text);
    if (!module_) {
        return CTRANIAN_ERROR_PARSE_FAILED;
    }
    
    return CTRANIAN_SUCCESS;
}

ctransian_error_t Translator::optimizeModule() {
    if (!module_) {
        return CTRANIAN_ERROR_PARSE_FAILED;
    }
    
    // Set optimization level
    BinaryenSetOptimizeLevel(static_cast<int>(config_->optimization_level));
    
    // Apply optimizations
    BinaryenModuleOptimize(module_);
    
    return CTRANIAN_SUCCESS;
}

ctransian_error_t Translator::analyzeModule() {
    if (!module_) {
        return CTRANIAN_ERROR_PARSE_FAILED;
    }
    
    // Reset module info
    std::memset(&module_info_, 0, sizeof(ctransian_module_info_t));
    
    // Extract basic information from module
    // This is a placeholder - actual implementation would use Binaryen APIs
    // to count functions, imports, exports, etc.
    
    module_analyzed_ = true;
    
    return CTRANIAN_SUCCESS;
}

ctransian_error_t Translator::generateCode(ctransian_result_t* result) {
    if (!module_ || !result) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    if (!module_analyzed_) {
        ctransian_error_t error = analyzeModule();
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
    }
    
    try {
        // Setup code generator
        code_generator_->setModule(module_);
        code_generator_->setTypeMapper(type_mapper_.get());
        code_generator_->setInstructionVisitor(instruction_visitor_.get());
        
        // Generate code
        ctransian_error_t error = code_generator_->generateModule(module_, result);
        if (error != CTRANIAN_SUCCESS) {
            return error;
        }
        
        // Update result statistics
        result->functions_translated = module_info_.num_functions;
        result->instructions_translated = module_info_.num_functions * 100; // Placeholder
        
        return CTRANIAN_SUCCESS;
        
    } catch (const std::exception& e) {
        last_error_ = e.what();
        return CTRANIAN_ERROR_TRANSLATION_FAILED;
    }
}

ctransian_error_t Translator::validateConfiguration() {
    if (!config_) {
        return CTRANIAN_ERROR_INVALID_CONFIG;
    }
    
    // Validate optimization level
    if (config_->optimization_level > CTRANIAN_OPTIMIZE_AGGRESSIVE) {
        return CTRANIAN_ERROR_INVALID_CONFIG;
    }
    
    // Validate C standard
    if (config_->target_c_standard > CTRANIAN_GNU11) {
        return CTRANIAN_ERROR_INVALID_CONFIG;
    }
    
    // Validate bounds checking mode
    if (config_->bounds_checking > CTRANIAN_BOUNDS_STRICT) {
        return CTRANIAN_ERROR_INVALID_CONFIG;
    }
    
    // Validate runtime type
    if (config_->runtime_type > CTRANIAN_RUNTIME_STANDALONE) {
        return CTRANIAN_ERROR_INVALID_CONFIG;
    }
    
    return CTRANIAN_SUCCESS;
}

void Translator::reset() {
    if (module_) {
        BinaryenModuleDispose(module_);
        module_ = nullptr;
    }
    
    module_analyzed_ = false;
    warnings_.clear();
    last_error_.clear();
}

std::string Translator::generateErrorMessage(BinaryenExpressionRef expr) {
    // Placeholder implementation
    return "Unknown error in Binaryen expression";
}

std::string Translator::getDiagnosticInfo() {
    // Return accumulated warnings and errors
    std::string diagnostics;
    for (const auto& warning : warnings_) {
        diagnostics += "Warning: " + warning + "\n";
    }
    if (!last_error_.empty()) {
        diagnostics += "Error: " + last_error_ + "\n";
    }
    return diagnostics;
}

// Utility function implementations
namespace utils {
    std::string escapeCIdentifier(const std::string& name) {
        std::string escaped;
        for (char c : name) {
            if (std::isalnum(c) || c == '_') {
                escaped += c;
            } else {
                escaped += '_';
            }
        }
        return escaped;
    }
    
    std::string sanitizeFunctionName(const std::string& name) {
        std::string sanitized = escapeCIdentifier(name);
        if (std::isdigit(sanitized[0])) {
            sanitized = "_" + sanitized;
        }
        return sanitized;
    }
    
    std::string generateUniqueName(const std::string& prefix) {
        static int counter = 0;
        return prefix + "_" + std::to_string(counter++);
    }
    
    bool isValidCIdentifier(const std::string& name) {
        if (name.empty() || std::isdigit(name[0])) {
            return false;
        }
        for (char c : name) {
            if (!std::isalnum(c) && c != '_') {
                return false;
            }
        }
        return true;
    }
}

} // namespace ctransian