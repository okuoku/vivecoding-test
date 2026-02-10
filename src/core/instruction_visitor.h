#ifndef CTRANIAN_INSTRUCTION_VISITOR_H
#define CTRANIAN_INSTRUCTION_VISITOR_H

#include <binaryen-c.h>
#include <string>
#include <vector>
#include <stack>
#include <memory>

#include "ctransian/public.h"
#include "type_mapper.h"

namespace ctransian {

/**
 * Visitor pattern implementation for WebAssembly instructions
 * Translates Binaryen IR expressions to equivalent C code
 */
class InstructionVisitor {
public:
    explicit InstructionVisitor(const TypeMapper* type_mapper, const ctransian_config_t* config);
    ~InstructionVisitor();

    // Main visitation method
    std::string visitExpression(BinaryenExpressionRef expr);
    
    // Instruction category visitors
    std::string visitConstant(BinaryenExpressionRef expr);
    std::string visitUnary(BinaryenExpressionRef expr);
    std::string visitBinary(BinaryenExpressionRef expr);
    std::string visitSelect(BinaryenExpressionRef expr);
    std::string visitDrop(BinaryenExpressionRef expr);
    std::string visitNop(BinaryenExpressionRef expr);
    std::string visitUnreachable(BinaryenExpressionRef expr);
    std::string visitLoad(BinaryenExpressionRef expr);
    std::string visitStore(BinaryenExpressionRef expr);
    std::string visitCall(BinaryenExpressionRef expr);
    std::string visitCallIndirect(BinaryenExpressionRef expr);
    std::string visitLocalGet(BinaryenExpressionRef expr);
    std::string visitLocalSet(BinaryenExpressionRef expr);
    std::string visitLocalTee(BinaryenExpressionRef expr);
    std::string visitGlobalGet(BinaryenExpressionRef expr);
    std::string visitGlobalSet(BinaryenExpressionRef expr);
    
    // Control flow visitors
    std::string visitBlock(BinaryenExpressionRef expr);
    std::string visitLoop(BinaryenExpressionRef expr);
    std::string visitIf(BinaryenExpressionRef expr);
    std::string visitBreak(BinaryenExpressionRef expr);
    std::string visitSwitch(BinaryenExpressionRef expr);
    std::string visitReturn(BinaryenExpressionRef expr);
    
    // Memory operators
    std::string visitMemorySize(BinaryenExpressionRef expr);
    std::string visitMemoryGrow(BinaryenExpressionRef expr);
    std::string visitMemoryFill(BinaryenExpressionRef expr);
    std::string visitMemoryCopy(BinaryenExpressionRef expr);
    std::string visitMemoryInit(BinaryenExpressionRef expr);
    
    // SIMD visitors
    std::string visitSimdExtract(BinaryenExpressionRef expr);
    std::string visitSimdReplace(BinaryenExpressionRef expr);
    std::string visitSimdShuffle(BinaryenExpressionRef expr);
    std::string visitSimdTernary(BinaryenExpressionRef expr);
    std::string visitSimdShift(BinaryenExpressionRef expr);
    std::string visitSimdLoad(BinaryenExpressionRef expr);
    std::string visitSimdStore(BinaryenExpressionRef expr);
    
    // Atomic visitors
    std::string visitAtomicLoad(BinaryenExpressionRef expr);
    std::string visitAtomicStore(BinaryenExpressionRef expr);
    std::string visitAtomicRMW(BinaryenExpressionRef expr);
    std::string visitAtomicCmpxchg(BinaryenExpressionRef expr);
    
    // Reference type visitors
    std::string visitRefNull(BinaryenExpressionRef expr);
    std::string visitRefIsNull(BinaryenExpressionRef expr);
    std::string visitRefFunc(BinaryenExpressionRef expr);
    
    // GC visitors (future implementation)
    std::string visitStructNew(BinaryenExpressionRef expr);
    std::string visitStructGet(BinaryenExpressionRef expr);
    std::string visitStructSet(BinaryenExpressionRef expr);
    std::string visitArrayNew(BinaryenExpressionRef expr);
    std::string visitArrayGet(BinaryenExpressionRef expr);
    std::string visitArraySet(BinaryenExpressionRef expr);
    std::string visitArrayLen(BinaryenExpressionRef expr);

    // State management
    void reset();
    void setCurrentFunction(const std::string& func_name);
    void enterBlock(const std::string& block_name);
    void exitBlock();

    // Code generation helpers
    std::string generateTempVariable(BinaryenType type);
    std::string generateLabel(const std::string& prefix);
    void emitCode(const std::string& code);
    void emitComment(const std::string& comment);

    // Get generated code
    const std::string& getCode() const { return generated_code_; }
    const std::vector<std::string>& getTemporaries() const { return temporaries_; }

private:
    // Helper methods
    std::string getOperationName(BinaryenExpressionRef expr);
    std::string generateExpressionList(BinaryenExpressionRef* exprs, size_t count);
    std::string generateFunctionCall(const std::string& func_name, 
                                   const std::vector<std::string>& args,
                                   BinaryenType return_type);
    
    // Type-aware code generation
    std::string generateTypeCast(const std::string& expr, 
                               BinaryenType from_type, 
                               BinaryenType to_type);
    std::string generateBoundsCheck(const std::string& address, 
                                  size_t access_size,
                                  bool is_atomic = false);
    
    // Memory access helpers
    std::string generateMemoryAccess(const std::string& address,
                                    size_t offset,
                                    size_t access_size,
                                    bool is_signed = false,
                                    bool is_atomic = false);
    size_t getTypeSize(BinaryenType type);
    
    // SIMD helpers
    std::string getSimdIntrinsic(const std::string& operation, BinaryenType type);
    std::string generateSimdConstant(BinaryenLiteral value);
    
    // Atomic helpers
    std::string getAtomicOperation(const std::string& op, size_t size);
    std::string generateMemoryOrder(ctransian_memory_order_t order);
    
    // State
    const TypeMapper* type_mapper_;
    const ctransian_config_t* config_;
    
    // Code generation state
    std::string generated_code_;
    std::vector<std::string> temporaries_;
    std::stack<std::string> block_stack_;
    std::string current_function_;
    int temp_counter_;
    int label_counter_;
    
    // Feature detection
    bool has_simd_;
    bool has_atomic_;
    bool has_gc_;
};

/**
 * Instruction utility functions
 */
namespace instruction_utils {
    // Expression utilities
    bool isConstantExpression(BinaryenExpressionRef expr);
    bool isSideEffectFree(BinaryenExpressionRef expr);
    BinaryenType getExpressionType(BinaryenExpressionRef expr);
    
    // Operation categorization
    bool isArithmeticOperation(BinaryenExpressionRef expr);
    bool isLogicalOperation(BinaryenExpressionRef expr);
    bool isMemoryOperation(BinaryenExpressionRef expr);
    bool isControlFlowOperation(BinaryenExpressionRef expr);
    
    // Optimization helpers
    bool canBeInlined(BinaryenExpressionRef expr);
    bool isPureFunction(const std::string& func_name);
    std::string optimizeConstantFolding(BinaryenExpressionRef expr);
}

} // namespace ctransian

#endif // CTRANIAN_INSTRUCTION_VISITOR_H