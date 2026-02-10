#ifndef CTRANIAN_CODE_GENERATOR_H
#define CTRANIAN_CODE_GENERATOR_H

#include <binaryen-c.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>

#include "ctransian/public.h"
#include "type_mapper.h"
#include "instruction_visitor.h"

namespace ctransian {

/**
 * Main C code generation engine
 * Coordinates all code generation activities and produces final C output
 */
class CodeGenerator {
public:
    explicit CodeGenerator(const ctransian_config_t* config);
    ~CodeGenerator();

    // Main generation methods
    ctransian_error_t generateModule(BinaryenModuleRef module, ctransian_result_t* result);
    ctransian_error_t generateFunction(BinaryenFunctionRef func, std::string& code);
    ctransian_error_t generateRuntimeSupport(std::string& header_code, std::string& impl_code);

    // Module-level generation
    void generateHeader();
    void generateIncludes();
    void generateTypeDefinitions();
    void generateGlobalDeclarations();
    void generateMemoryDeclarations();
    void generateTableDeclarations();
    void generateFunctionDeclarations();
    void generateExportDeclarations();

    // Implementation generation
    void generateFunctionImplementations();
    void generateRuntimeImplementation();
    void generateInitializationCode();
    void generateCleanupCode();

    // Utility generation
    void generateDebugInfo();
    void generateComments();
    void generateMacros();

    // Configuration and state
    void setModule(BinaryenModuleRef module);
    void setTypeMapper(const TypeMapper* type_mapper);
    void setInstructionVisitor(InstructionVisitor* visitor);

    // Output management
    const std::string& getHeaderCode() const { return header_code_; }
    const std::string& getSourceCode() const { return source_code_; }
    const std::string& getRuntimeCode() const { return runtime_code_; }

private:
    // Generation helpers
    std::string generateFunctionSignature(BinaryenFunctionRef func);
    std::string generateFunctionBody(BinaryenFunctionRef func);
    std::string generateVariableDeclarations(BinaryenFunctionRef func);
    std::string generateParameterList(BinaryenFunctionRef func);
    std::string generateReturnStatement(BinaryenType return_type);

    // Memory generation
    void generateLinearMemory();
    void generateMemoryAccessFunctions();
    void generateBoundsCheckingMacros();
    void generateAtomicMemoryFunctions();

    // Table generation
    void generateFunctionTables();
    void generateIndirectCallHelpers();

    // Global generation
    void generateGlobalVariables();
    void generateGlobalInitialization();

    // Import/Export generation
    void generateImportStubs();
    void generateExportWrappers();

    // Threading support
    void generateThreadSupport();
    void generateAtomicOperations();
    void generateSynchronizationPrimitives();

    // SIMD support
    void generateSimdTypes();
    void generateSimdIntrinsics();
    void generateSimdUtilities();

    // GC support (future)
    void generateGCTypes();
    void generateGCUtilities();
    void generateGCRuntime();

    // WASI support
    void generateWASIStubs();
    void generateWASIHelpers();

    // Code formatting and optimization
    void formatCode();
    void optimizeGeneratedCode();
    void addDebugAnnotations();

    // File generation helpers
    std::string generateFileHeader(const std::string& filename);
    std::string generateGuardMacros(const std::string& guard_name);
    std::string generateCopyrightNotice();

    // Configuration
    const ctransian_config_t* config_;
    
    // Translation components
    BinaryenModuleRef module_;
    const TypeMapper* type_mapper_;
    InstructionVisitor* instruction_visitor_;
    
    // Generated code storage
    std::string header_code_;
    std::string source_code_;
    std::string runtime_code_;
    
    // Generation state
    std::vector<std::string> function_names_;
    std::vector<std::string> global_names_;
    std::vector<std::string> type_names_;
    std::unordered_map<std::string, std::string> generated_symbols_;
    
    // Statistics
    uint32_t functions_generated_;
    uint32_t lines_of_code_;
    size_t total_code_size_;
};

/**
 * Code generation utilities
 */
namespace codegen_utils {
    // String formatting
    std::string indentString(const std::string& code, int indent_level);
    std::string formatCCode(const std::string& code);
    std::string wrapInComment(const std::string& comment, const std::string& prefix = "//");
    
    // Identifier generation
    std::string generateCIdentifier(const std::string& wasm_name);
    std::string generateMangledName(const std::string& base_name, const std::string& suffix);
    std::string generateUniqueIdentifier(const std::string& prefix);
    
    // Code patterns
    std::string generateFunctionCallPattern(const std::string& func, const std::vector<std::string>& args);
    std::string generateAssignmentPattern(const std::string& lhs, const std::string& rhs);
    std::string generateConditionalPattern(const std::string& condition, 
                                        const std::string& then_branch, 
                                        const std::string& else_branch = "");
    
    // Type-aware generation
    std::string generateTypeCast(const std::string& expr, const std::string& target_type);
    std::string generateDefaultValue(const std::string& type);
    std::string generateConstantExpression(const std::string& value, const std::string& type);
    
    // Error handling
    std::string generateErrorCheck(const std::string& condition, const std::string& error_msg);
    std::string generatePanicCode(const std::string& message);
    std::string generateAssertion(const std::string& condition, const std::string& message);
    
    // Performance utilities
    std::string generateInlineHint();
    std::string generateNoReturnHint();
    std::string generateCompilerHint(const std::string& hint);
}

/**
 * Code optimization passes
 */
namespace optimization {
    // Basic optimizations
    void removeUnusedVariables(std::string& code);
    void optimizeConstantFolding(std::string& code);
    void eliminateDeadCode(std::string& code);
    
    // Advanced optimizations
    void inlineSmallFunctions(std::string& code);
    void optimizeLoops(std::string& code);
    void optimizeMemoryAccess(std::string& code);
    
    // Platform-specific optimizations
    void optimizeForTarget(std::string& code, const std::string& target);
    void optimizeSimdCode(std::string& code);
    void optimizeAtomicCode(std::string& code);
}

} // namespace ctransian

#endif // CTRANIAN_CODE_GENERATOR_H