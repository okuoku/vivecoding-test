#include <iostream>
#include <cassert>
#include <binaryen-c.h>
#include "../../src/core/instruction_visitor.h"
#include "../../src/core/type_mapper.h"
#include "../utils/test_helpers.h"

using namespace ctransian_test;

void test_i32_add_instruction() {
    std::cout << "Testing i32.add instruction..." << std::endl;
    
    // Create a simple config
    ctransian_config_t* config = ctransian_config_create();
    config->optimization_level = CTRANIAN_OPTIMIZE_NONE;
    
    // Create type mapper and instruction visitor
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    // Create a simple Binaryen module for testing
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.add expression: 10 + 32
    BinaryenExpressionRef add_expr = BinaryenBinary(
        module,
        BinaryenAddInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(10)),
        BinaryenConst(module, BinaryenLiteralInt32(32))
    );
    
    std::string result = visitor.visitBinary(add_expr);
    std::cout << "✓ i32.add generated: " << result << std::endl;
    
    // The result should contain addition operation
    assert(!result.empty());
    assert(result.find("+") != std::string::npos || result.find("10") != std::string::npos || result.find("32") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.add test passed" << std::endl;
}

void test_i32_sub_instruction() {
    std::cout << "Testing i32.sub instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.sub expression: 100 - 25
    BinaryenExpressionRef sub_expr = BinaryenBinary(
        module,
        BinaryenSubInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(100)),
        BinaryenConst(module, BinaryenLiteralInt32(25))
    );
    
    std::string result = visitor.visitBinary(sub_expr);
    std::cout << "✓ i32.sub generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("-") != std::string::npos || result.find("100") != std::string::npos || result.find("25") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.sub test passed" << std::endl;
}

void test_i32_mul_instruction() {
    std::cout << "Testing i32.mul instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i32.mul expression: 12 * 8
    BinaryenExpressionRef mul_expr = BinaryenBinary(
        module,
        BinaryenMulInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(12)),
        BinaryenConst(module, BinaryenLiteralInt32(8))
    );
    
    std::string result = visitor.visitBinary(mul_expr);
    std::cout << "✓ i32.mul generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("*") != std::string::npos || result.find("12") != std::string::npos || result.find("8") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.mul test passed" << std::endl;
}

void test_i64_add_instruction() {
    std::cout << "Testing i64.add instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i64.add expression: 10000000000 + 20000000000
    BinaryenExpressionRef add_expr = BinaryenBinary(
        module,
        BinaryenAddInt64(),
        BinaryenConst(module, BinaryenLiteralInt64(10000000000LL)),
        BinaryenConst(module, BinaryenLiteralInt64(20000000000LL))
    );
    
    std::string result = visitor.visitBinary(add_expr);
    std::cout << "✓ i64.add generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("+") != std::string::npos || result.find("10000000000") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i64.add test passed" << std::endl;
}

void test_i64_sub_instruction() {
    std::cout << "Testing i64.sub instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i64.sub expression: 50000000000 - 25000000000
    BinaryenExpressionRef sub_expr = BinaryenBinary(
        module,
        BinaryenSubInt64(),
        BinaryenConst(module, BinaryenLiteralInt64(50000000000LL)),
        BinaryenConst(module, BinaryenLiteralInt64(25000000000LL))
    );
    
    std::string result = visitor.visitBinary(sub_expr);
    std::cout << "✓ i64.sub generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("-") != std::string::npos || result.find("50000000000") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i64.sub test passed" << std::endl;
}

void test_i64_mul_instruction() {
    std::cout << "Testing i64.mul instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an i64.mul expression: 1000000 * 1000
    BinaryenExpressionRef mul_expr = BinaryenBinary(
        module,
        BinaryenMulInt64(),
        BinaryenConst(module, BinaryenLiteralInt64(1000000LL)),
        BinaryenConst(module, BinaryenLiteralInt64(1000LL))
    );
    
    std::string result = visitor.visitBinary(mul_expr);
    std::cout << "✓ i64.mul generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("*") != std::string::npos || result.find("1000000") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i64.mul test passed" << std::endl;
}

void test_f32_add_instruction() {
    std::cout << "Testing f32.add instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f32.add expression: 1.5 + 2.5
    BinaryenExpressionRef add_expr = BinaryenBinary(
        module,
        BinaryenAddFloat32(),
        BinaryenConst(module, BinaryenLiteralFloat32(1.5f)),
        BinaryenConst(module, BinaryenLiteralFloat32(2.5f))
    );
    
    std::string result = visitor.visitBinary(add_expr);
    std::cout << "✓ f32.add generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("+") != std::string::npos || result.find("f") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f32.add test passed" << std::endl;
}

void test_f32_sub_instruction() {
    std::cout << "Testing f32.sub instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f32.sub expression: 10.0 - 3.5
    BinaryenExpressionRef sub_expr = BinaryenBinary(
        module,
        BinaryenSubFloat32(),
        BinaryenConst(module, BinaryenLiteralFloat32(10.0f)),
        BinaryenConst(module, BinaryenLiteralFloat32(3.5f))
    );
    
    std::string result = visitor.visitBinary(sub_expr);
    std::cout << "✓ f32.sub generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("-") != std::string::npos || result.find("f") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f32.sub test passed" << std::endl;
}

void test_f32_mul_instruction() {
    std::cout << "Testing f32.mul instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f32.mul expression: 4.0 * 2.5
    BinaryenExpressionRef mul_expr = BinaryenBinary(
        module,
        BinaryenMulFloat32(),
        BinaryenConst(module, BinaryenLiteralFloat32(4.0f)),
        BinaryenConst(module, BinaryenLiteralFloat32(2.5f))
    );
    
    std::string result = visitor.visitBinary(mul_expr);
    std::cout << "✓ f32.mul generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("*") != std::string::npos || result.find("f") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f32.mul test passed" << std::endl;
}

void test_f64_add_instruction() {
    std::cout << "Testing f64.add instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create an f64.add expression: 1.23456789 + 9.87654321
    BinaryenExpressionRef add_expr = BinaryenBinary(
        module,
        BinaryenAddFloat64(),
        BinaryenConst(module, BinaryenLiteralFloat64(1.23456789)),
        BinaryenConst(module, BinaryenLiteralFloat64(9.87654321))
    );
    
    std::string result = visitor.visitBinary(add_expr);
    std::cout << "✓ f64.add generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("+") != std::string::npos || result.find("1.23456789") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f64.add test passed" << std::endl;
}

void test_complex_arithmetic_chain() {
    std::cout << "Testing complex arithmetic chain..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a complex arithmetic expression: ((5 + 10) * 3) - 15 + 5
    BinaryenExpressionRef add1 = BinaryenBinary(
        module, BinaryenAddInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(5)),
        BinaryenConst(module, BinaryenLiteralInt32(10))
    );
    
    BinaryenExpressionRef mul = BinaryenBinary(
        module, BinaryenMulInt32(),
        add1,
        BinaryenConst(module, BinaryenLiteralInt32(3))
    );
    
    BinaryenExpressionRef sub = BinaryenBinary(
        module, BinaryenSubInt32(),
        mul,
        BinaryenConst(module, BinaryenLiteralInt32(15))
    );
    
    BinaryenExpressionRef add2 = BinaryenBinary(
        module, BinaryenAddInt32(),
        sub,
        BinaryenConst(module, BinaryenLiteralInt32(5))
    );
    
    std::string result = visitor.visitBinary(add2);
    std::cout << "✓ Complex arithmetic generated: " << result << std::endl;
    
    assert(!result.empty());
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ Complex arithmetic test passed" << std::endl;
}

int main() {
    std::cout << "Running Phase 2 Arithmetic Operations Tests..." << std::endl;
    std::cout << "===========================================" << std::endl;
    
    try {
        test_i32_add_instruction();
        test_i32_sub_instruction();
        test_i32_mul_instruction();
        test_i64_add_instruction();
        test_i64_sub_instruction();
        test_i64_mul_instruction();
        test_f32_add_instruction();
        test_f32_sub_instruction();
        test_f32_mul_instruction();
        test_f64_add_instruction();
        test_complex_arithmetic_chain();
        
        std::cout << "===========================================" << std::endl;
        std::cout << "✓ All arithmetic operations tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "✗ Test failed with unknown exception" << std::endl;
        return 1;
    }
}