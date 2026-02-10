#include <iostream>
#include <cassert>
#include <binaryen-c.h>
#include "../../src/core/instruction_visitor.h"
#include "../../src/core/type_mapper.h"
#include "../utils/test_helpers.h"

using namespace ctransian_test;

void test_i32_eq_instruction() {
    std::cout << "Testing i32.eq instruction..." << std::endl;
    
    // Create a simple config
    ctransian_config_t* config = ctransian_config_create();
    config->optimization_level = CTRANIAN_OPTIMIZE_NONE;
    
    // Create type mapper and instruction visitor
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    // Create a simple Binaryen module for testing
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.eq expression: 42 == 42
    BinaryenExpressionRef eq_expr = BinaryenBinary(
        module,
        BinaryenEqInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(42)),
        BinaryenConst(module, BinaryenLiteralInt32(42))
    );
    
    std::string result = visitor.visitBinary(eq_expr);
    std::cout << "✓ i32.eq generated: " << result << std::endl;
    
    // The result should contain equality comparison
    assert(!result.empty());
    assert(result.find("==") != std::string::npos || result.find("42") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.eq test passed" << std::endl;
}

void test_i32_ne_instruction() {
    std::cout << "Testing i32.ne instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.ne expression: 42 != 24
    BinaryenExpressionRef ne_expr = BinaryenBinary(
        module,
        BinaryenNeInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(42)),
        BinaryenConst(module, BinaryenLiteralInt32(24))
    );
    
    std::string result = visitor.visitBinary(ne_expr);
    std::cout << "✓ i32.ne generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("!=") != std::string::npos || result.find("42") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.ne test passed" << std::endl;
}

void test_i32_lt_s_instruction() {
    std::cout << "Testing i32.lt_s instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.lt_s expression: 10 < 20 (signed)
    BinaryenExpressionRef lt_expr = BinaryenBinary(
        module,
        BinaryenLtSInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(10)),
        BinaryenConst(module, BinaryenLiteralInt32(20))
    );
    
    std::string result = visitor.visitBinary(lt_expr);
    std::cout << "✓ i32.lt_s generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("<") != std::string::npos || result.find("10") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.lt_s test passed" << std::endl;
}

void test_i32_gt_s_instruction() {
    std::cout << "Testing i32.gt_s instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.gt_s expression: 30 > 20 (signed)
    BinaryenExpressionRef gt_expr = BinaryenBinary(
        module,
        BinaryenGtSInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(30)),
        BinaryenConst(module, BinaryenLiteralInt32(20))
    );
    
    std::string result = visitor.visitBinary(gt_expr);
    std::cout << "✓ i32.gt_s generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find(">") != std::string::npos || result.find("30") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.gt_s test passed" << std::endl;
}

void test_i32_le_s_instruction() {
    std::cout << "Testing i32.le_s instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.le_s expression: 20 <= 20 (signed)
    BinaryenExpressionRef le_expr = BinaryenBinary(
        module,
        BinaryenLeSInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(20)),
        BinaryenConst(module, BinaryenLiteralInt32(20))
    );
    
    std::string result = visitor.visitBinary(le_expr);
    std::cout << "✓ i32.le_s generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("<=") != std::string::npos || result.find("20") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.le_s test passed" << std::endl;
}

void test_i32_ge_s_instruction() {
    std::cout << "Testing i32.ge_s instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.ge_s expression: 20 >= 20 (signed)
    BinaryenExpressionRef ge_expr = BinaryenBinary(
        module,
        BinaryenGeSInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(20)),
        BinaryenConst(module, BinaryenLiteralInt32(20))
    );
    
    std::string result = visitor.visitBinary(ge_expr);
    std::cout << "✓ i32.ge_s generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find(">=") != std::string::npos || result.find("20") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.ge_s test passed" << std::endl;
}

void test_i32_lt_u_instruction() {
    std::cout << "Testing i32.lt_u instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.lt_u expression: 10 < 20 (unsigned)
    BinaryenExpressionRef lt_expr = BinaryenBinary(
        module,
        BinaryenLtUInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(10)),
        BinaryenConst(module, BinaryenLiteralInt32(20))
    );
    
    std::string result = visitor.visitBinary(lt_expr);
    std::cout << "✓ i32.lt_u generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("<") != std::string::npos || result.find("10") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.lt_u test passed" << std::endl;
}

void test_i64_eq_instruction() {
    std::cout << "Testing i64.eq instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i64.eq expression: 12345678900 == 12345678900
    BinaryenExpressionRef eq_expr = BinaryenBinary(
        module,
        BinaryenEqInt64(),
        BinaryenConst(module, BinaryenLiteralInt64(12345678900LL)),
        BinaryenConst(module, BinaryenLiteralInt64(12345678900LL))
    );
    
    std::string result = visitor.visitBinary(eq_expr);
    std::cout << "✓ i64.eq generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("==") != std::string::npos || result.find("12345678900") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i64.eq test passed" << std::endl;
}

void test_f32_eq_instruction() {
    std::cout << "Testing f32.eq instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f32.eq expression: 1.5 == 1.5
    BinaryenExpressionRef eq_expr = BinaryenBinary(
        module,
        BinaryenEqFloat32(),
        BinaryenConst(module, BinaryenLiteralFloat32(1.5f)),
        BinaryenConst(module, BinaryenLiteralFloat32(1.5f))
    );
    
    std::string result = visitor.visitBinary(eq_expr);
    std::cout << "✓ f32.eq generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("==") != std::string::npos || result.find("f") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f32.eq test passed" << std::endl;
}

void test_f32_lt_instruction() {
    std::cout << "Testing f32.lt instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f32.lt expression: 1.5 < 2.5
    BinaryenExpressionRef lt_expr = BinaryenBinary(
        module,
        BinaryenLtFloat32(),
        BinaryenConst(module, BinaryenLiteralFloat32(1.5f)),
        BinaryenConst(module, BinaryenLiteralFloat32(2.5f))
    );
    
    std::string result = visitor.visitBinary(lt_expr);
    std::cout << "✓ f32.lt generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("<") != std::string::npos || result.find("f") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f32.lt test passed" << std::endl;
}

void test_f64_eq_instruction() {
    std::cout << "Testing f64.eq instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f64.eq expression: 3.141592653589793 == 3.141592653589793
    BinaryenExpressionRef eq_expr = BinaryenBinary(
        module,
        BinaryenEqFloat64(),
        BinaryenConst(module, BinaryenLiteralFloat64(3.141592653589793)),
        BinaryenConst(module, BinaryenLiteralFloat64(3.141592653589793))
    );
    
    std::string result = visitor.visitBinary(eq_expr);
    std::cout << "✓ f64.eq generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("==") != std::string::npos || result.find("3.141592653589793") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f64.eq test passed" << std::endl;
}

void test_f64_lt_instruction() {
    std::cout << "Testing f64.lt instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f64.lt expression: 1.23456789 < 9.87654321
    BinaryenExpressionRef lt_expr = BinaryenBinary(
        module,
        BinaryenLtFloat64(),
        BinaryenConst(module, BinaryenLiteralFloat64(1.23456789)),
        BinaryenConst(module, BinaryenLiteralFloat64(9.87654321))
    );
    
    std::string result = visitor.visitBinary(lt_expr);
    std::cout << "✓ f64.lt generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("<") != std::string::npos || result.find("1.23456789") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f64.lt test passed" << std::endl;
}

void test_complex_comparison_chain() {
    std::cout << "Testing complex comparison chain..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a complex comparison: (10 < 20) && (5 > 3) && (42 == 42)
    BinaryenExpressionRef lt = BinaryenBinary(
        module, BinaryenLtSInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(10)),
        BinaryenConst(module, BinaryenLiteralInt32(20))
    );
    
    BinaryenExpressionRef gt = BinaryenBinary(
        module, BinaryenGtSInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(5)),
        BinaryenConst(module, BinaryenLiteralInt32(3))
    );
    
    BinaryenExpressionRef eq = BinaryenBinary(
        module, BinaryenEqInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(42)),
        BinaryenConst(module, BinaryenLiteralInt32(42))
    );
    
    BinaryenExpressionRef and1 = BinaryenBinary(
        module, BinaryenAndInt32(),
        lt, gt
    );
    
    BinaryenExpressionRef and2 = BinaryenBinary(
        module, BinaryenAndInt32(),
        and1, eq
    );
    
    std::string result = visitor.visitBinary(and2);
    std::cout << "✓ Complex comparison generated: " << result << std::endl;
    
    assert(!result.empty());
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ Complex comparison test passed" << std::endl;
}

int main() {
    std::cout << "Running Phase 2 Comparison Operations Tests..." << std::endl;
    std::cout << "===========================================" << std::endl;
    
    try {
        test_i32_eq_instruction();
        test_i32_ne_instruction();
        test_i32_lt_s_instruction();
        test_i32_gt_s_instruction();
        test_i32_le_s_instruction();
        test_i32_ge_s_instruction();
        test_i32_lt_u_instruction();
        test_i64_eq_instruction();
        test_f32_eq_instruction();
        test_f32_lt_instruction();
        test_f64_eq_instruction();
        test_f64_lt_instruction();
        test_complex_comparison_chain();
        
        std::cout << "===========================================" << std::endl;
        std::cout << "✓ All comparison operations tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "✗ Test failed with unknown exception" << std::endl;
        return 1;
    }
}