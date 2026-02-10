#include "instruction_visitor.h"
#include <cassert>
#include <cmath>
#include <string>

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
    return "/* block */";
}

std::string InstructionVisitor::visitLoop(BinaryenExpressionRef expr) {
    return "/* loop */";
}

std::string InstructionVisitor::visitIf(BinaryenExpressionRef expr) {
    return "/* if */";
}

std::string InstructionVisitor::visitSelect(BinaryenExpressionRef expr) {
    return "/* select */";
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
    return "/* local.get */";
}

std::string InstructionVisitor::visitLocalSet(BinaryenExpressionRef expr) {
    return "/* local.set */";
}

std::string InstructionVisitor::visitLocalTee(BinaryenExpressionRef expr) {
    return "/* local.tee */";
}

std::string InstructionVisitor::visitGlobalGet(BinaryenExpressionRef expr) {
    return "/* global.get */";
}

std::string InstructionVisitor::visitGlobalSet(BinaryenExpressionRef expr) {
    return "/* global.set */";
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