#include <iostream>
#include <cassert>
#include <binaryen-c.h>
#include "../../src/core/instruction_visitor.h"
#include "../../src/core/type_mapper.h"
#include "../utils/test_helpers.h"

using namespace ctransian_test;

void test_i32_load_instruction() {
    std::cout << "Testing i32.load instruction..." << std::endl;
    
    // Create a simple config
    ctransian_config_t* config = ctransian_config_create();
    config->optimization_level = CTRANIAN_OPTIMIZE_NONE;
    
    // Create type mapper and instruction visitor
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    // Create a simple Binaryen module for testing
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a memory load expression: i32.load offset=0 align=4
    BinaryenExpressionRef load_expr = BinaryenLoad(
        module, 
        4,  // bytes
        true,  // signed
        0,   // offset
        4,   // align
        BinaryenTypeInt32(),
        BinaryenConst(module, BinaryenLiteralInt32(0))  // address pointer
    );
    
    std::string result = visitor.visitLoad(load_expr);
    std::cout << "✓ i32.load generated: " << result << std::endl;
    
    // The result should contain memory loading code with bounds checking
    assert(!result.empty());
    assert(result.find("memory") != std::string::npos || result.find("load") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.load test passed" << std::endl;
}

void test_i64_load_instruction() {
    std::cout << "Testing i64.load instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a 64-bit memory load expression
    BinaryenExpressionRef load_expr = BinaryenLoad(
        module,
        8,  // bytes (64-bit)
        true,
        0,
        8,  // align (64-bit)
        BinaryenTypeInt64(),
        BinaryenConst(module, BinaryenLiteralInt32(8))  // address pointer with offset
    );
    
    std::string result = visitor.visitLoad(load_expr);
    std::cout << "✓ i64.load generated: " << result << std::endl;
    
    assert(!result.empty());
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i64.load test passed" << std::endl;
}

void test_f32_load_instruction() {
    std::cout << "Testing f32.load instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a 32-bit float memory load expression
    BinaryenExpressionRef load_expr = BinaryenLoad(
        module,
        4,  // bytes (32-bit float)
        false,  // unsigned for floats
        0,
        4,  // align
        BinaryenTypeFloat32(),
        BinaryenConst(module, BinaryenLiteralInt32(16))  // address pointer with offset
    );
    
    std::string result = visitor.visitLoad(load_expr);
    std::cout << "✓ f32.load generated: " << result << std::endl;
    
    assert(!result.empty());
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f32.load test passed" << std::endl;
}

void test_f64_load_instruction() {
    std::cout << "Testing f64.load instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a 64-bit float memory load expression
    BinaryenExpressionRef load_expr = BinaryenLoad(
        module,
        8,  // bytes (64-bit double)
        false,
        0,
        8,  // align
        BinaryenTypeFloat64(),
        BinaryenConst(module, BinaryenLiteralInt32(24))  // address pointer with offset
    );
    
    std::string result = visitor.visitLoad(load_expr);
    std::cout << "✓ f64.load generated: " << result << std::endl;
    
    assert(!result.empty());
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ f64.load test passed" << std::endl;
}

void test_i32_store_instruction() {
    std::cout << "Testing i32.store instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a memory store expression: i32.store value=42 offset=0 align=4
    BinaryenExpressionRef store_expr = BinaryenStore(
        module,
        4,  // bytes
        0,  // offset
        4,  // align
        BinaryenConst(module, BinaryenLiteralInt32(0)),  // address
        BinaryenConst(module, BinaryenLiteralInt32(42))   // value
    );
    
    std::string result = visitor.visitStore(store_expr);
    std::cout << "✓ i32.store generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("42") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i32.store test passed" << std::endl;
}

void test_i64_store_instruction() {
    std::cout << "Testing i64.store instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a 64-bit memory store expression
    BinaryenExpressionRef store_expr = BinaryenStore(
        module,
        8,  // bytes (64-bit)
        8,  // offset
        8,  // align
        BinaryenConst(module, BinaryenLiteralInt32(8)),   // address
        BinaryenConst(module, BinaryenLiteralInt64(12345678900LL))  // value
    );
    
    std::string result = visitor.visitStore(store_expr);
    std::cout << "✓ i64.store generated: " << result << std::endl;
    
    assert(!result.empty());
    assert(result.find("12345678900") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ i64.store test passed" << std::endl;
}

void test_memory_size_instruction() {
    std::cout << "Testing memory.size instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a memory.size expression
    BinaryenExpressionRef memsize_expr = BinaryenMemorySize(module);
    
    std::string result = visitor.visitMemorySize(memsize_expr);
    std::cout << "✓ memory.size generated: " << result << std::endl;
    
    assert(!result.empty());
    // Should call runtime function to get memory size
    assert(result.find("memory") != std::string::npos || result.find("size") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ memory.size test passed" << std::endl;
}

void test_memory_grow_instruction() {
    std::cout << "Testing memory.grow instruction..." << std::endl;
    
    ctransian_config_t* config = ctransian_config_create();
    ctransian::TypeMapper type_mapper(config);
    ctransian::InstructionVisitor visitor(&type_mapper, config);
    
    BinaryenModuleRef module = BinaryenModuleCreate();
    
    // Create a memory.grow expression - grow by 1 page
    BinaryenExpressionRef memgrow_expr = BinaryenMemoryGrow(
        module,
        BinaryenConst(module, BinaryenLiteralInt32(1))  // pages to grow
    );
    
    std::string result = visitor.visitMemoryGrow(memgrow_expr);
    std::cout << "✓ memory.grow generated: " << result << std::endl;
    
    assert(!result.empty());
    // Should call runtime function to grow memory
    assert(result.find("memory") != std::string::npos || result.find("grow") != std::string::npos);
    
    BinaryenModuleDispose(module);
    ctransian_config_destroy(config);
    std::cout << "✓ memory.grow test passed" << std::endl;
}

int main() {
    std::cout << "Running Phase 2 Memory Operations Tests..." << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        test_i32_load_instruction();
        test_i64_load_instruction();
        test_f32_load_instruction();
        test_f64_load_instruction();
        test_i32_store_instruction();
        test_i64_store_instruction();
        test_memory_size_instruction();
        test_memory_grow_instruction();
        
        std::cout << "========================================" << std::endl;
        std::cout << "✓ All memory operations tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "✗ Test failed with unknown exception" << std::endl;
        return 1;
    }
}