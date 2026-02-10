#include "instruction_visitor.h"
#include <cassert>
#include <cmath>
#include <string>
#include <cstring>

namespace ctransian {

InstructionVisitor::InstructionVisitor(const TypeMapper* type_mapper, const ctransian_config_t* config) 
    : type_mapper_(type_mapper), config_(config) {
}

InstructionVisitor::~InstructionVisitor() {
    // Cleanup if needed
}

std::string InstructionVisitor::visitExpression(BinaryenExpressionRef expr) {
    BinaryenExpressionId id = BinaryenExpressionGetId(expr);
    
    // Use if-else instead of switch since Binaryen ID functions are not constexpr
    if (id == BinaryenConstId()) {
        return visitConstant(expr);
    } else if (id == BinaryenUnaryId()) {
        return visitUnary(expr);
    } else if (id == BinaryenBinaryId()) {
        return visitBinary(expr);
    } else if (id == BinaryenLoadId()) {
        return visitLoad(expr);
    } else if (id == BinaryenStoreId()) {
        return visitStore(expr);
    } else if (id == BinaryenCallId()) {
        return visitCall(expr);
    } else if (id == BinaryenBlockId()) {
        return visitBlock(expr);
    } else if (id == BinaryenLoopId()) {
        return visitLoop(expr);
    } else if (id == BinaryenIfId()) {
        return visitIf(expr);
    } else if (id == BinaryenSelectId()) {
        return visitSelect(expr);
    } else if (id == BinaryenDropId()) {
        return visitDrop(expr);
    } else if (id == BinaryenNopId()) {
        return visitNop(expr);
    } else if (id == BinaryenUnreachableId()) {
        return visitUnreachable(expr);
    } else if (id == BinaryenBreakId()) {
        return visitBreak(expr);
    } else if (id == BinaryenSwitchId()) {
        return visitSwitch(expr);
    } else if (id == BinaryenReturnId()) {
        return visitReturn(expr);
    } else if (id == BinaryenMemorySizeId()) {
        return visitMemorySize(expr);
    } else if (id == BinaryenMemoryGrowId()) {
        return visitMemoryGrow(expr);
    } else if (id == BinaryenLocalGetId()) {
        return visitLocalGet(expr);
    } else if (id == BinaryenLocalSetId()) {
        return visitLocalSet(expr);
    // Note: LocalTee might be handled differently or not supported in this Binaryen version
    } else if (id == BinaryenGlobalGetId()) {
        return visitGlobalGet(expr);
    } else if (id == BinaryenGlobalSetId()) {
        return visitGlobalSet(expr);
    } else {
        return "/* unsupported expression */";
    }
}

std::string InstructionVisitor::visitConstant(BinaryenExpressionRef expr) {
    BinaryenType type = BinaryenExpressionGetType(expr);
    
    // Handle different constant types
    if (type == BinaryenTypeInt32()) {
        int32_t value = BinaryenConstGetValueI32(expr);
        return std::to_string(value);
    } else if (type == BinaryenTypeInt64()) {
        int64_t value = BinaryenConstGetValueI64(expr);
        return std::to_string(value) + "LL";
    } else if (type == BinaryenTypeFloat32()) {
        float value = BinaryenConstGetValueF32(expr);
        // Generate proper C float literal
        if (std::isnan(value)) {
            return "NAN";
        } else if (std::isinf(value)) {
            return value > 0 ? "INFINITY" : "-INFINITY";
        } else {
            std::string result = std::to_string(value);
            // Ensure 'f' suffix for float literals
            if (result.find('.') == std::string::npos && result.find('e') == std::string::npos) {
                result += ".0";
            }
            return result + "f";
        }
    } else if (type == BinaryenTypeFloat64()) {
        double value = BinaryenConstGetValueF64(expr);
        // Generate proper C double literal
        if (std::isnan(value)) {
            return "NAN";
        } else if (std::isinf(value)) {
            return value > 0 ? "INFINITY" : "-INFINITY";
        } else {
            std::string result = std::to_string(value);
            // Ensure we have a decimal point for double literals
            if (result.find('.') == std::string::npos && result.find('e') == std::string::npos) {
                result += ".0";
            }
            return result;
        }
    } else if (type == BinaryenTypeVec128()) {
        // Handle SIMD vectors - for now return a placeholder
        // Full implementation would extract all 16 bytes and format them
        return "/* v128 constant */";
    } else {
        return "/* unsupported constant type */";
    }
}

std::string InstructionVisitor::visitUnary(BinaryenExpressionRef expr) {
    return "/* unary operation */";
}

std::string InstructionVisitor::visitBinary(BinaryenExpressionRef expr) {
    // Get left and right operands
    BinaryenExpressionRef left = BinaryenBinaryGetLeft(expr);
    BinaryenExpressionRef right = BinaryenBinaryGetRight(expr);
    
    // Get the binary operation type
    BinaryenOp op = BinaryenBinaryGetOp(expr);
    
    // Generate code for operands
    std::string left_str = visitExpression(left);
    std::string right_str = visitExpression(right);
    
    // Generate C code based on the operation using if-else instead of switch
    // Integer arithmetic
    if (op == BinaryenAddInt32() || op == BinaryenAddInt64() ||
        op == BinaryenAddFloat32() || op == BinaryenAddFloat64()) {
        return "(" + left_str + " + " + right_str + ")";
    }
    if (op == BinaryenSubInt32() || op == BinaryenSubInt64() ||
        op == BinaryenSubFloat32() || op == BinaryenSubFloat64()) {
        return "(" + left_str + " - " + right_str + ")";
    }
    if (op == BinaryenMulInt32() || op == BinaryenMulInt64() ||
        op == BinaryenMulFloat32() || op == BinaryenMulFloat64()) {
        return "(" + left_str + " * " + right_str + ")";
    }
    
    // Integer comparison
    if (op == BinaryenEqInt32() || op == BinaryenEqInt64() ||
        op == BinaryenEqFloat32() || op == BinaryenEqFloat64()) {
        return "(" + left_str + " == " + right_str + ")";
    }
    if (op == BinaryenNeInt32() || op == BinaryenNeInt64() ||
        op == BinaryenNeFloat32() || op == BinaryenNeFloat64()) {
        return "(" + left_str + " != " + right_str + ")";
    }
    if (op == BinaryenLtSInt32() || op == BinaryenLtSInt64() ||
        op == BinaryenLtUInt32() || op == BinaryenLtUInt64() ||
        op == BinaryenLtFloat32() || op == BinaryenLtFloat64()) {
        return "(" + left_str + " < " + right_str + ")";
    }
    if (op == BinaryenGtSInt32() || op == BinaryenGtSInt64() ||
        op == BinaryenGtFloat32() || op == BinaryenGtFloat64()) {
        return "(" + left_str + " > " + right_str + ")";
    }
    if (op == BinaryenLeSInt32() || op == BinaryenLeSInt64() ||
        op == BinaryenLeFloat32() || op == BinaryenLeFloat64()) {
        return "(" + left_str + " <= " + right_str + ")";
    }
    if (op == BinaryenGeSInt32() || op == BinaryenGeSInt64() ||
        op == BinaryenGeFloat32() || op == BinaryenGeFloat64()) {
        return "(" + left_str + " >= " + right_str + ")";
    }
            
    // Bitwise operations (basic ones for Phase 2)
    if (op == BinaryenAndInt32() || op == BinaryenAndInt64()) {
        return "(" + left_str + " & " + right_str + ")";
    }
    if (op == BinaryenOrInt32() || op == BinaryenOrInt64()) {
        return "(" + left_str + " | " + right_str + ")";
    }
    if (op == BinaryenXorInt32() || op == BinaryenXorInt64()) {
        return "(" + left_str + " ^ " + right_str + ")";
    }
        
    return "/* unsupported binary op */";
}

std::string InstructionVisitor::visitLoad(BinaryenExpressionRef expr) {
    // Get the memory address expression
    BinaryenExpressionRef addr = BinaryenLoadGetPtr(expr);
    
    // Get offset and alignment
    BinaryenIndex offset = BinaryenLoadGetOffset(expr);
    BinaryenIndex align = BinaryenLoadGetAlign(expr);
    
    // Get the result type of the load
    BinaryenType result_type = BinaryenExpressionGetType(expr);
    
    // Check if it's signed
    bool is_signed = BinaryenLoadIsSigned(expr);
    
    // Generate code for the address
    std::string addr_str = visitExpression(addr);
    
    // Calculate final address
    std::string final_addr = "(" + addr_str;
    if (offset > 0) {
        final_addr += " + " + std::to_string(offset);
    }
    final_addr += ")";
    
    // Generate bounds check (if enabled in config)
    std::string bounds_check = "";
    if (config_ && config_->bounds_checking != CTRANIAN_BOUNDS_NONE) {
        bounds_check = generateBoundsCheck(final_addr, getTypeSize(result_type));
    }
    
    // Get the C type
    std::string c_type = "int32_t"; // Default fallback
    if (type_mapper_) {
        c_type = const_cast<TypeMapper*>(type_mapper_)->wasmTypeToCType(result_type);
    }
    
    // Generate the load operation
    std::string load_expr = "*(";
    if (is_signed && (result_type == BinaryenTypeInt32() || result_type == BinaryenTypeInt64())) {
        load_expr += "const ";
    }
    load_expr += c_type + "*)(" + final_addr + ")";
    
    // Combine bounds check and load
    if (!bounds_check.empty()) {
        return bounds_check + "; " + load_expr;
    } else {
        return load_expr;
    }
}

std::string InstructionVisitor::visitStore(BinaryenExpressionRef expr) {
    // Get the memory address expression
    BinaryenExpressionRef addr = BinaryenStoreGetPtr(expr);
    
    // Get the value being stored
    BinaryenExpressionRef value = BinaryenStoreGetValue(expr);
    
    // Get offset and alignment
    BinaryenIndex offset = BinaryenStoreGetOffset(expr);
    BinaryenIndex align = BinaryenStoreGetAlign(expr);
    
    // Get stored value type
    BinaryenType value_type = BinaryenStoreGetValueType(expr);
    
    // Generate code for address and value
    std::string addr_str = visitExpression(addr);
    std::string value_str = visitExpression(value);
    
    // Calculate final address
    std::string final_addr = "(" + addr_str;
    if (offset > 0) {
        final_addr += " + " + std::to_string(offset);
    }
    final_addr += ")";
    
    // Generate bounds check (if enabled in config)
    std::string bounds_check = "";
    if (config_ && config_->bounds_checking != CTRANIAN_BOUNDS_NONE) {
        bounds_check = generateBoundsCheck(final_addr, getTypeSize(value_type));
    }
    
    // Get the C type
    std::string c_type = "int32_t"; // Default fallback
    if (type_mapper_) {
        c_type = const_cast<TypeMapper*>(type_mapper_)->wasmTypeToCType(value_type);
    }
    
    // Generate store operation
    std::string store_expr = "*(" + c_type + "*)(" + final_addr + ") = " + value_str;
    
    // Combine bounds check and store
    if (!bounds_check.empty()) {
        return bounds_check + "; " + store_expr;
    } else {
        return store_expr;
    }
}

std::string ctransian::InstructionVisitor::visitCall(BinaryenExpressionRef expr) {
    // Get the target function name
    const char* func_name = BinaryenCallGetTarget(expr);
    
    // Get the number of operands
    BinaryenIndex num_operands = BinaryenCallGetNumOperands(expr);
    
    // Generate the function name
    std::string call_expr = std::string(func_name) + "(";
    
    // Generate arguments
    for (BinaryenIndex i = 0; i < num_operands; ++i) {
        if (i > 0) {
            call_expr += ", ";
        }
        
        BinaryenExpressionRef operand = BinaryenCallGetOperandAt(expr, i);
        std::string operand_str = visitExpression(operand);
        call_expr += operand_str;
    }
    
    call_expr += ")";
    
    return call_expr;
}

std::string ctransian::InstructionVisitor::visitBlock(BinaryenExpressionRef expr) {
    // Get block name (label) - may be empty
    const char* block_name = BinaryenBlockGetName(expr);
    
    // Get number of children in the block
    BinaryenIndex num_children = BinaryenBlockGetNumChildren(expr);
    
    std::string result;
    
    // If block has a name, create a label
    if (block_name && strlen(block_name) > 0) {
        result += "/* block: " + std::string(block_name) + " */ {\n";
    } else {
        result += "/* anonymous block */ {\n";
    }
    
    // Generate code for each child expression
    for (BinaryenIndex i = 0; i < num_children; ++i) {
        BinaryenExpressionRef child = BinaryenBlockGetChildAt(expr, i);
        std::string child_code = visitExpression(child);
        result += "  " + child_code + ";\n";
    }
    
    result += "}";
    
    return result;
}

std::string InstructionVisitor::visitLoop(BinaryenExpressionRef expr) {
    // Get loop name (label) - may be empty
    const char* loop_name = BinaryenLoopGetName(expr);
    
    // Get the body expression
    BinaryenExpressionRef body = BinaryenLoopGetBody(expr);
    
    std::string result;
    
    // If loop has a name, create a label
    if (loop_name && strlen(loop_name) > 0) {
        result += "/* loop: " + std::string(loop_name) + " */ while (1) {\n";
    } else {
        result += "/* anonymous loop */ while (1) {\n";
    }
    
    // Generate code for the body
    std::string body_code = visitExpression(body);
    result += "  " + body_code + ";\n";
    result += "}";
    
    return result;
}

std::string InstructionVisitor::visitIf(BinaryenExpressionRef expr) {
    // Get condition, ifTrue, and ifFalse expressions
    BinaryenExpressionRef condition = BinaryenIfGetCondition(expr);
    BinaryenExpressionRef if_true = BinaryenIfGetIfTrue(expr);
    BinaryenExpressionRef if_false = BinaryenIfGetIfFalse(expr);
    
    // Generate code for condition
    std::string condition_str = visitExpression(condition);
    
    // Generate code for ifTrue branch
    std::string if_true_str = visitExpression(if_true);
    
    std::string result = "if (" + condition_str + ") {\n";
    result += "  " + if_true_str + ";\n";
    
    // Check if there's an else branch
    if (if_false) {
        std::string if_false_str = visitExpression(if_false);
        result += "} else {\n";
        result += "  " + if_false_str + ";\n";
    }
    
    result += "}";
    
    return result;
}

std::string InstructionVisitor::visitSelect(BinaryenExpressionRef expr) {
    // Get the condition, ifTrue, and ifFalse expressions
    BinaryenExpressionRef condition = BinaryenSelectGetCondition(expr);
    BinaryenExpressionRef if_true = BinaryenSelectGetIfTrue(expr);
    BinaryenExpressionRef if_false = BinaryenSelectGetIfFalse(expr);
    
    // Generate C code for each operand
    std::string condition_str = visitExpression(condition);
    std::string if_true_str = visitExpression(if_true);
    std::string if_false_str = visitExpression(if_false);
    
    // Generate ternary operator: condition ? if_true : if_false
    return "(" + condition_str + " ? " + if_true_str + " : " + if_false_str + ")";
}

std::string InstructionVisitor::visitDrop(BinaryenExpressionRef expr) {
    // Get the value being dropped
    BinaryenExpressionRef value = BinaryenDropGetValue(expr);
    
    // Generate the expression but don't use the result
    // This effectively evaluates the expression and discards the result
    std::string value_str = visitExpression(value);
    
    // Cast to void to indicate the value is intentionally unused
    return "(void)(" + value_str + ")";
}

std::string InstructionVisitor::visitNop(BinaryenExpressionRef expr) {
    // NOP does nothing - return empty statement
    return "/* nop */";
}

std::string InstructionVisitor::visitUnreachable(BinaryenExpressionRef expr) {
    // Unreachable should generate code that will never be reached
    // In C, we can use a call to a runtime trap function
    return "ctransian_trap()";
}

std::string InstructionVisitor::visitBreak(BinaryenExpressionRef expr) {
    // Get the target label name (empty for implicit break)
    const char* label_name = BinaryenBreakGetName(expr);
    
    // Get the value being passed to the break target (if any)
    BinaryenExpressionRef value = BinaryenBreakGetValue(expr);
    
    std::string result;
    
    // Generate the value expression if present
    if (value) {
        std::string value_str = visitExpression(value);
        if (label_name && strlen(label_name) > 0) {
            result = "goto label_" + std::string(label_name) + "_break";
        } else {
            result = "break";
        }
        // Store the value in a temporary before breaking if needed
        result = "/* value: " + value_str + "; */ " + result;
    } else {
        if (label_name && strlen(label_name) > 0) {
            result = "goto label_" + std::string(label_name) + "_break";
        } else {
            result = "break";
        }
    }
    
    return result;
}

std::string InstructionVisitor::visitReturn(BinaryenExpressionRef expr) {
    // Get the return value (if any)
    BinaryenExpressionRef value = BinaryenReturnGetValue(expr);
    
    if (value) {
        // Generate code for the return value
        std::string value_str = visitExpression(value);
        return "return " + value_str;
    } else {
        // Void return
        return "return";
    }
}

std::string InstructionVisitor::visitMemorySize(BinaryenExpressionRef expr) {
    // memory.size returns the current memory size in pages
    // We need to call the runtime function to get the current memory size
    return "ctransian_memory_size()";
}

std::string InstructionVisitor::visitMemoryGrow(BinaryenExpressionRef expr) {
    // Get the number of pages to grow
    BinaryenExpressionRef delta = BinaryenMemoryGrowGetDelta(expr);
    
    // Generate code for the delta expression
    std::string delta_str = visitExpression(delta);
    
    // memory.grow returns the previous size or -1 on failure
    // We need to call the runtime function to grow memory
    return "ctransian_memory_grow(" + delta_str + ")";
}

std::string InstructionVisitor::visitLocalGet(BinaryenExpressionRef expr) {
    // Get the local variable index
    BinaryenIndex index = BinaryenLocalGetGetIndex(expr);
    
    // Generate local variable name (using a simple naming convention)
    std::string var_name = "local_" + std::to_string(index);
    
    return var_name;
}

std::string InstructionVisitor::visitLocalSet(BinaryenExpressionRef expr) {
    // Get the local variable index
    BinaryenIndex index = BinaryenLocalSetGetIndex(expr);
    
    // Get the value being assigned
    BinaryenExpressionRef value = BinaryenLocalSetGetValue(expr);
    
    // Generate code for the value expression
    std::string value_str = visitExpression(value);
    
    // Generate local variable name
    std::string var_name = "local_" + std::to_string(index);
    
    // Generate assignment statement
    return var_name + " = " + value_str;
}

std::string InstructionVisitor::visitLocalTee(BinaryenExpressionRef expr) {
    // LocalTee is like LocalSet but also returns the value
    // Get the local variable index
    BinaryenIndex index = BinaryenLocalSetGetIndex(expr);  // Uses same accessor as LocalSet
    
    // Get the value being assigned
    BinaryenExpressionRef value = BinaryenLocalSetGetValue(expr);
    
    // Generate code for the value expression
    std::string value_str = visitExpression(value);
    
    // Generate local variable name
    std::string var_name = "local_" + std::to_string(index);
    
    // For local.tee, we need to assign and also return the value
    // We can use a comma expression: (local_x = value, value)
    return "(" + var_name + " = " + value_str + ", " + value_str + ")";
}

std::string InstructionVisitor::visitGlobalGet(BinaryenExpressionRef expr) {
    // Get the global variable name
    const char* global_name = BinaryenGlobalGetGetName(expr);
    
    // Generate global variable access
    // Use a prefix to avoid conflicts with C keywords
    return "global_" + std::string(global_name);
}

std::string InstructionVisitor::visitGlobalSet(BinaryenExpressionRef expr) {
    // Get the global variable name
    const char* global_name = BinaryenGlobalSetGetName(expr);
    
    // Get the value being assigned
    BinaryenExpressionRef value = BinaryenGlobalSetGetValue(expr);
    
    // Generate code for the value expression
    std::string value_str = visitExpression(value);
    
    // Generate global variable name with prefix
    std::string var_name = "global_" + std::string(global_name);
    
    // Generate assignment statement
    return var_name + " = " + value_str;
}

std::string InstructionVisitor::visitSwitch(BinaryenExpressionRef expr) {
    // Get the condition expression
    BinaryenExpressionRef condition = BinaryenSwitchGetCondition(expr);
    
    // Get the number of targets
    BinaryenIndex num_targets = BinaryenSwitchGetNumNames(expr);
    
    // Get the default target
    const char* default_target = BinaryenSwitchGetDefaultName(expr);
    
    std::string condition_str = visitExpression(condition);
    std::string result = "switch (" + condition_str + ") {\n";
    
    // Generate case statements for each target
    for (BinaryenIndex i = 0; i < num_targets; ++i) {
        const char* target_name = BinaryenSwitchGetNameAt(expr, i);
        result += "  case " + std::to_string(i) + ": goto label_" + std::string(target_name) + "_break;\n";
    }
    
    // Generate default case
    result += "  default: goto label_" + std::string(default_target) + "_break;\n";
    result += "}";
    
    return result;
}

std::string InstructionVisitor::generateBoundsCheck(const std::string& address, size_t access_size, bool is_atomic) {
    // Generate bounds check: if address is out of bounds, trap
    return "CTRANIAN_BOUNDS_CHECK(" + address + ", " + std::to_string(access_size) + ")";
}

size_t InstructionVisitor::getTypeSize(BinaryenType type) {
    // Fallback size calculations
    if (type == BinaryenTypeInt32() || type == BinaryenTypeFloat32()) return 4;
    if (type == BinaryenTypeInt64() || type == BinaryenTypeFloat64()) return 8;
    if (type == BinaryenTypeVec128()) return 16;
    return 0; // Unknown type
}

std::string InstructionVisitor::generateMemoryOrder(ctransian_memory_order_t order) {
    switch (order) {
        case CTRANIAN_MEMORY_ORDER_RELAXED: return "memory_order_relaxed";
        case CTRANIAN_MEMORY_ORDER_ACQUIRE: return "memory_order_acquire";
        case CTRANIAN_MEMORY_ORDER_RELEASE: return "memory_order_release";
        case CTRANIAN_MEMORY_ORDER_ACQ_REL: return "memory_order_acq_rel";
        case CTRANIAN_MEMORY_ORDER_SEQ_CST: return "memory_order_seq_cst";
        default: return "memory_order_seq_cst";
    }
}

std::string InstructionVisitor::visitAtomicLoad(BinaryenExpressionRef expr) {
    return "/* atomic.load */";
}

std::string InstructionVisitor::visitAtomicStore(BinaryenExpressionRef expr) {
    return "/* atomic.store */";
}

std::string InstructionVisitor::visitAtomicRMW(BinaryenExpressionRef expr) {
    return "/* atomic.rmw */";
}

std::string InstructionVisitor::visitAtomicCmpxchg(BinaryenExpressionRef expr) {
    return "/* atomic.cmpxchg */";
}

std::string InstructionVisitor::visitSimdExtract(BinaryenExpressionRef expr) {
    return "/* simd.extract */";
}

std::string InstructionVisitor::visitSimdReplace(BinaryenExpressionRef expr) {
    return "/* simd.replace */";
}

std::string InstructionVisitor::visitSimdShuffle(BinaryenExpressionRef expr) {
    return "/* simd.shuffle */";
}

std::string InstructionVisitor::visitSimdTernary(BinaryenExpressionRef expr) {
    return "/* simd.ternary */";
}

std::string InstructionVisitor::visitSimdLoad(BinaryenExpressionRef expr) {
    return "/* simd.load */";
}

std::string InstructionVisitor::visitSimdStore(BinaryenExpressionRef expr) {
    return "/* simd.store */";
}

} // namespace ctransian