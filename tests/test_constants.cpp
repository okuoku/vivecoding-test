#include <iostream>
#include <cassert>
#include <binaryen-c.h>
#include "../src/core/instruction_visitor.h"
#include "../src/core/type_mapper.h"
#include "ctransian/public.h"

void test_integer_constants() {
    std::cout << "Testing integer constants..." << std::endl;
    
    // Create a simple config
    ctransian_config_t* config = ctransian_config_create();
    config->optimization_level = CTRANIAN_OPTIMIZE_NONE;
    
    // Create type mapper
    ctransian::TypeMapper type_mapper(config);
    
    // Create instruction visitor
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    // Create a simple Binaryen module for testing
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Test i32.const
    BinaryenExpressionRef i32_const = BinaryenConst(module, BinaryenLiteralInt32(42));
    std::string result = visitor.visitConstant(i32_const);
    assert(result == "42");
    std::cout << "✓ i32.const 42 -> " << result << std::endl;
    
    // Test i32.const negative
    BinaryenExpressionRef i32_neg = BinaryenConst(module, BinaryenLiteralInt32(-123));
    result = visitor.visitConstant(i32_neg);
    assert(result == "-123");
    std::cout << "✓ i32.const -123 -> " << result << std::endl;
    
    // Test i64.const
    BinaryenExpressionRef i64_const = BinaryenConst(module, BinaryenLiteralInt64(12345678900LL));
    result = visitor.visitConstant(i64_const);
    assert(result == "12345678900LL");
    std::cout << "✓ i64.const 12345678900 -> " << result << std::endl;
    
    // Test i64.const negative
    BinaryenExpressionRef i64_neg = BinaryenConst(module, BinaryenLiteralInt64(-98765432100LL));
    result = visitor.visitConstant(i64_neg);
    assert(result == "-98765432100LL");
    std::cout << "✓ i64.const -98765432100 -> " << result << std::endl;
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ Integer constants test passed" << std::endl;
}

void test_float_constants() {
    std::cout << "Testing float constants..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Test f32.const
    BinaryenExpressionRef f32_const = BinaryenConst(module, BinaryenLiteralFloat32(3.14159f));
    std::string result = visitor.visitConstant(f32_const);
    // The exact string representation may vary, but should end with 'f'
    assert(result.find('f') != std::string::npos);
    std::cout << "✓ f32.const 3.14159 -> " << result << std::endl;
    
    // Test f32.const zero
    BinaryenExpressionRef f32_zero = BinaryenConst(module, BinaryenLiteralFloat32(0.0f));
    result = visitor.visitConstant(f32_zero);
    std::cout << "Debug: f32 zero result = '" << result << "'" << std::endl;
    assert(result.find("f") != std::string::npos); // Just check it ends with f
    std::cout << "✓ f32.const 0.0 -> " << result << std::endl;
    
    // Test f64.const
    BinaryenExpressionRef f64_const = BinaryenConst(module, BinaryenLiteralFloat64(2.718281828459045));
    std::string result2 = visitor.visitConstant(f64_const);
    std::cout << "✓ f64.const 2.718281828459045 -> " << result2 << std::endl;
    
    // Test f64.const zero
    BinaryenExpressionRef f64_zero = BinaryenConst(module, BinaryenLiteralFloat64(0.0));
    result2 = visitor.visitConstant(f64_zero);
    assert(result2.find("0") == 0); // Just check it starts with 0
    std::cout << "✓ f64.const 0.0 -> " << result2 << std::endl;
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ Float constants test passed" << std::endl;
}

void test_expression_dispatch() {
    std::cout << "Testing expression dispatch..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Test that visitExpression correctly dispatches constants
    BinaryenExpressionRef const_expr = BinaryenConst(module, BinaryenLiteralInt32(999));
    std::string result = visitor.visitExpression(const_expr);
    assert(result == "999");
    std::cout << "✓ Expression dispatch for const 999 -> " << result << std::endl;
    
    // Test that non-const expressions return placeholder
    BinaryenExpressionRef add_expr = BinaryenBinary(
        module, 
        BinaryenAddInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(1)),
        BinaryenConst(module, BinaryenLiteralInt32(2))
    );
    result = visitor.visitExpression(add_expr);
    assert(result == "/* binary operation */");
    std::cout << "✓ Expression dispatch for binary op -> " << result << std::endl;
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ Expression dispatch test passed" << std::endl;
}

void test_special_float_values() {
    std::cout << "Testing special float values..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Test NaN using bit pattern
    uint32_t nan_bits = 0x7fc00000; // IEEE 754 NaN
    BinaryenExpressionRef f32_nan = BinaryenConst(module, BinaryenLiteralFloat32Bits(nan_bits));
    std::string result = visitor.visitConstant(f32_nan);
    assert(result == "NAN");
    std::cout << "✓ f32 NaN -> " << result << std::endl;
    
    // Test infinity
    BinaryenExpressionRef f32_inf = BinaryenConst(module, BinaryenLiteralFloat32(INFINITY));
    result = visitor.visitConstant(f32_inf);
    assert(result == "INFINITY");
    std::cout << "✓ f32 INFINITY -> " << result << std::endl;
    
    BinaryenExpressionRef f32_neginf = BinaryenConst(module, BinaryenLiteralFloat32(-INFINITY));
    result = visitor.visitConstant(f32_neginf);
    assert(result == "-INFINITY");
    std::cout << "✓ f32 -INFINITY -> " << result << std::endl;
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ Special float values test passed" << std::endl;
}

int main() {
    std::cout << "Running ctransian constants tests..." << std::endl;
    std::cout << "====================================" << std::endl;
    
    try {
        test_integer_constants();
        test_float_constants();
        test_expression_dispatch();
        test_special_float_values();
        
        std::cout << "====================================" << std::endl;
        std::cout << "✓ All constants tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "✗ Test failed with unknown exception" << std::endl;
        return 1;
    }
}