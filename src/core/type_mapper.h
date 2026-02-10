#ifndef CTRANIAN_TYPE_MAPPER_H
#define CTRANIAN_TYPE_MAPPER_H

#include <binaryen-c.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "ctransian/public.h"

namespace ctransian {

/**
 * Maps WebAssembly types to C types and vice versa
 */
class TypeMapper {
public:
    explicit TypeMapper(const ctransian_config_t* config);
    ~TypeMapper() = default;

    // Core type mapping
    std::string wasmTypeToCType(BinaryenType wasm_type);
    BinaryenType cTypeToWasmType(const std::string& c_type);

    // Function type mapping
    std::string generateFunctionSignature(BinaryenFunctionRef func_ref, const std::string& name);
    std::string getReturnTypeString(BinaryenType return_type);
    std::vector<std::string> getParameterTypeStrings(BinaryenType param_types);

    // Value type mapping
    std::string getValueTypeString(BinaryenType value_type);
    std::string getDefaultValue(BinaryenType type);

    // Complex type handling
    std::string generateStructDefinition(const std::string& name, const std::vector<BinaryenType>& fields);
    std::string generateArrayDefinition(const std::string& name, BinaryenType element_type);
    std::string generateVectorTypeString(BinaryenType vector_type);

    // SIMD type mapping
    std::string getSimdIntrinsicType(BinaryenType type);
    std::string getSimdOperationName(const std::string& wasm_op);

    // Reference type mapping
    std::string getReferenceTypeString(BinaryenType ref_type);
    std::string getGCTypeString(BinaryenType gc_type);

    // Type validation
    bool isSupportedType(BinaryenType type);
    bool requiresSpecialHandling(BinaryenType type);

    // Type utilities
    std::string getTypePrefix(BinaryenType type);
    std::string getTypeSuffix(BinaryenType type);
    size_t getTypeSize(BinaryenType type);

    // C standard specific mappings
    std::string getC11AtomicType(BinaryenType type);
    std::string getThreadLocalType(BinaryenType type);

private:
    // Basic type mappings
    void initializeBasicTypeMappings();
    void initializeSimdTypeMappings();
    void initializeReferenceTypeMappings();

    // Helper methods
    std::string mapIntegerType(BinaryenType type);
    std::string mapFloatType(BinaryenType type);
    std::string mapVectorType(BinaryenType type);
    std::string mapReferenceType(BinaryenType type);

    // Configuration
    const ctransian_config_t* config_;

    // Type mapping tables
    std::unordered_map<BinaryenType, std::string> basic_type_map_;
    std::unordered_map<BinaryenType, std::string> simd_type_map_;
    std::unordered_map<BinaryenType, std::string> reference_type_map_;
    
    // C to WASM type mappings
    std::unordered_map<std::string, BinaryenType> c_to_wasm_map_;
    
    // Type information cache
    std::unordered_map<BinaryenType, size_t> type_sizes_;
    std::unordered_map<BinaryenType, std::string> default_values_;
};

/**
 * Type mapping utilities
 */
namespace type_utils {
    // Type checking utilities
    bool isIntegerType(BinaryenType type);
    bool isFloatType(BinaryenType type);
    bool isVectorType(BinaryenType type);
    bool isReferenceType(BinaryenType type);
    bool isMultiValueType(BinaryenType type);

    // Type size utilities
    size_t getWasmTypeSize(BinaryenType type);
    size_t getAlignmentRequirement(BinaryenType type);

    // Type conversion utilities
    std::string getConversionFunction(BinaryenType from, BinaryenType to);
    bool needsExplicitConversion(BinaryenType from, BinaryenType to);

    // C type utilities
    std::string getAtomicEquivalent(const std::string& c_type);
    std::string getVolatileEquivalent(const std::string& c_type);
    std::string getConstEquivalent(const std::string& c_type);
}

} // namespace ctransian

#endif // CTRANIAN_TYPE_MAPPER_H