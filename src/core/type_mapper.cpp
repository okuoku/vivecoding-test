#include "type_mapper.h"
#include <cassert>

namespace ctransian {

TypeMapper::TypeMapper(const ctransian_config_t* config) : config_(config) {
    initializeBasicTypeMappings();
    initializeSimdTypeMappings();
    initializeReferenceTypeMappings();
}

void TypeMapper::initializeBasicTypeMappings() {
    // Basic WASM to C type mappings
    basic_type_map_[BinaryenTypeInt32()] = "int32_t";
    basic_type_map_[BinaryenTypeInt64()] = "int64_t";
    basic_type_map_[BinaryenTypeFloat32()] = "float";
    basic_type_map_[BinaryenTypeFloat64()] = "double";
    
    // Initialize default values
    default_values_[BinaryenTypeInt32()] = "0";
    default_values_[BinaryenTypeInt64()] = "0LL";
    default_values_[BinaryenTypeFloat32()] = "0.0f";
    default_values_[BinaryenTypeFloat64()] = "0.0";
    
    // Initialize type sizes
    type_sizes_[BinaryenTypeInt32()] = 4;
    type_sizes_[BinaryenTypeInt64()] = 8;
    type_sizes_[BinaryenTypeFloat32()] = 4;
    type_sizes_[BinaryenTypeFloat64()] = 8;
}

void TypeMapper::initializeSimdTypeMappings() {
#ifdef CTRANIAN_ENABLE_SIMD
    simd_type_map_[BinaryenTypeVec128()] = "v128_t";
#endif
}

void TypeMapper::initializeReferenceTypeMappings() {
    reference_type_map_[BinaryenTypeFuncref()] = "void*";
    reference_type_map_[BinaryenTypeExternref()] = "void*";
}

std::string TypeMapper::wasmTypeToCType(BinaryenType wasm_type) {
    auto it = basic_type_map_.find(wasm_type);
    if (it != basic_type_map_.end()) {
        return it->second;
    }
    
    // Check SIMD types
    auto simd_it = simd_type_map_.find(wasm_type);
    if (simd_it != simd_type_map_.end()) {
        return simd_it->second;
    }
    
    // Check reference types
    auto ref_it = reference_type_map_.find(wasm_type);
    if (ref_it != reference_type_map_.end()) {
        return ref_it->second;
    }
    
    return "void"; // Default fallback
}

BinaryenType TypeMapper::cTypeToWasmType(const std::string& c_type) {
    auto it = c_to_wasm_map_.find(c_type);
    if (it != c_to_wasm_map_.end()) {
        return it->second;
    }
    return BinaryenTypeNone(); // Default fallback
}

std::string TypeMapper::generateFunctionSignature(BinaryenFunctionRef func_ref, const std::string& name) {
    // Get function type from Binaryen
    // This is a simplified implementation - full implementation would query Binaryen for function signature
    return "void " + name + "(void)"; // Placeholder
}

std::string TypeMapper::getReturnTypeString(BinaryenType return_type) {
    // Check for basic types first
    if (return_type == BinaryenTypeInt32()) return "int32_t";
    if (return_type == BinaryenTypeInt64()) return "int64_t";
    if (return_type == BinaryenTypeFloat32()) return "float";
    if (return_type == BinaryenTypeFloat64()) return "double";
    if (return_type == BinaryenTypeVec128()) return "v128_t";
    
    return "void"; // Default fallback
}

std::vector<std::string> TypeMapper::getParameterTypeStrings(BinaryenType param_types) {
    // This is simplified - full implementation would handle multi-value types
    std::vector<std::string> result;
    
    // Check for basic types
    if (param_types == BinaryenTypeInt32()) result.push_back("int32_t");
    else if (param_types == BinaryenTypeInt64()) result.push_back("int64_t");
    else if (param_types == BinaryenTypeFloat32()) result.push_back("float");
    else if (param_types == BinaryenTypeFloat64()) result.push_back("double");
    else if (param_types == BinaryenTypeVec128()) result.push_back("v128_t");
    
    return result;
}

std::string TypeMapper::getValueTypeString(BinaryenType value_type) {
    return wasmTypeToCType(value_type);
}

std::string TypeMapper::getDefaultValue(BinaryenType type) {
    auto it = default_values_.find(type);
    if (it != default_values_.end()) {
        return it->second;
    }
    return "0"; // Default fallback
}

bool TypeMapper::isSupportedType(BinaryenType type) {
    return basic_type_map_.find(type) != basic_type_map_.end() ||
           simd_type_map_.find(type) != simd_type_map_.end() ||
           reference_type_map_.find(type) != reference_type_map_.end();
}

bool TypeMapper::requiresSpecialHandling(BinaryenType type) {
    // SIMD, reference types, and multi-value types require special handling
    return simd_type_map_.find(type) != simd_type_map_.end() ||
           reference_type_map_.find(type) != reference_type_map_.end();
}

size_t TypeMapper::getTypeSize(BinaryenType type) {
    auto it = type_sizes_.find(type);
    if (it != type_sizes_.end()) {
        return it->second;
    }
    return 0; // Unknown type size
}

std::string TypeMapper::getC11AtomicType(BinaryenType type) {
    std::string base_type = wasmTypeToCType(type);
    return "_Atomic(" + base_type + ")";
}

std::string TypeMapper::getThreadLocalType(BinaryenType type) {
    std::string base_type = wasmTypeToCType(type);
    return "_Thread_local " + base_type;
}

// Utility functions
std::string TypeMapper::mapIntegerType(BinaryenType type) {
    if (type == BinaryenTypeInt32()) return "int32_t";
    if (type == BinaryenTypeInt64()) return "int64_t";
    return "int32_t"; // Default
}

std::string TypeMapper::mapFloatType(BinaryenType type) {
    if (type == BinaryenTypeFloat32()) return "float";
    if (type == BinaryenTypeFloat64()) return "double";
    return "float"; // Default
}

std::string TypeMapper::mapVectorType(BinaryenType type) {
#ifdef CTRANIAN_ENABLE_SIMD
    if (type == BinaryenTypeVec128()) return "v128_t";
#endif
    return "void"; // Fallback
}

std::string TypeMapper::mapReferenceType(BinaryenType type) {
    if (type == BinaryenTypeFuncref() || type == BinaryenTypeExternref()) {
        return "void*";
    }
    return "void*"; // Default
}

// Type utility functions namespace
namespace type_utils {

bool isIntegerType(BinaryenType type) {
    return type == BinaryenTypeInt32() || type == BinaryenTypeInt64();
}

bool isFloatType(BinaryenType type) {
    return type == BinaryenTypeFloat32() || type == BinaryenTypeFloat64();
}

bool isVectorType(BinaryenType type) {
    return type == BinaryenTypeVec128();
}

bool isReferenceType(BinaryenType type) {
    return type == BinaryenTypeFuncref() || type == BinaryenTypeExternref();
}

bool isMultiValueType(BinaryenType type) {
    // Binaryen doesn't directly expose multi-value types in C API
    // This would need additional implementation
    return false;
}

size_t getWasmTypeSize(BinaryenType type) {
    if (type == BinaryenTypeInt32() || type == BinaryenTypeFloat32()) return 4;
    if (type == BinaryenTypeInt64() || type == BinaryenTypeFloat64()) return 8;
    if (type == BinaryenTypeVec128()) return 16;
    return 0;
}

size_t getAlignmentRequirement(BinaryenType type) {
    return getWasmTypeSize(type); // Align to size for these types
}

std::string getConversionFunction(BinaryenType from, BinaryenType to) {
    // Simplified conversion mapping
    if (isIntegerType(from) && isFloatType(to)) {
        return "int2float";
    } else if (isFloatType(from) && isIntegerType(to)) {
        return "float2int";
    }
    return "";
}

bool needsExplicitConversion(BinaryenType from, BinaryenType to) {
    return !getConversionFunction(from, to).empty();
}

std::string getAtomicEquivalent(const std::string& c_type) {
    return "_Atomic(" + c_type + ")";
}

std::string getVolatileEquivalent(const std::string& c_type) {
    return "volatile " + c_type;
}

std::string getConstEquivalent(const std::string& c_type) {
    return "const " + c_type;
}

} // namespace type_utils

} // namespace ctransian