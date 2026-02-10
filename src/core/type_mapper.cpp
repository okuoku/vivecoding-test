#include "ctransian/type_mapper.h"
#include <cassert>

namespace ctransian {

TypeMapper::TypeMapper() {
    // Initialize primitive type mappings
    type_mappings_[BinaryenTypeInt32] = "int32_t";
    type_mappings_[BinaryenTypeInt64] = "int64_t";
    type_mappings_[BinaryenTypeFloat32] = "float";
    type_mappings_[BinaryenTypeFloat64] = "double";
}

std::string TypeMapper::map_wasm_type_to_c(BinaryenType wasm_type) const {
    auto it = type_mappings_.find(wasm_type);
    if (it != type_mappings_.end()) {
        return it->second;
    }
    
    // Handle multi-value types
    if (BinaryenTypeIsMultiValue(wasm_type)) {
        return "void"; // For now, handle as void - will need proper multi-value support
    }
    
    return "void";
}

std::string TypeMapper::generate_function_signature(BinaryenFunction* func) const {
    // This will be implemented when we have proper Binaryen function access
    return "void generated_function()";
}

} // namespace ctransian