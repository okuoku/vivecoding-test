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
    return "/* binary operation */";
}

std::string InstructionVisitor::visitLoad(BinaryenExpressionRef expr) {
    return "/* load operation */";
}

std::string InstructionVisitor::visitStore(BinaryenExpressionRef expr) {
    return "/* store operation */";
}

std::string InstructionVisitor::visitCall(BinaryenExpressionRef expr) {
    return "/* function call */";
}

std::string InstructionVisitor::visitBlock(BinaryenExpressionRef expr) {
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
    return "/* break */";
}

std::string InstructionVisitor::visitReturn(BinaryenExpressionRef expr) {
    return "/* return */";
}

std::string InstructionVisitor::visitMemorySize(BinaryenExpressionRef expr) {
    return "/* memory.size */";
}

std::string InstructionVisitor::visitMemoryGrow(BinaryenExpressionRef expr) {
    return "/* memory.grow */";
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