# Tests CMakeLists.txt

# Find WABT for wat2wasm conversion
find_program(WAT2WASM_EXECUTABLE wat2wasm PATHS ${CMAKE_SOURCE_DIR}/ext/wabt/bin)

if(NOT WAT2WASM_EXECUTABLE)
    message(FATAL_ERROR "wat2wasm not found. Make sure WABT is built and available")
endif()

# Test cases
set(WAT_FILES
    nop_test.wat
    if_test.wat
    loop_test.wat
    branch_test.wat
    return_test.wat
    block_test.wat
)

# Convert .wat files to .wasm
foreach(wat_file ${WAT_FILES})
    get_filename_component(name ${wat_file} NAME_WE)
    set(wasm_file "${name}.wasm")
    
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${wasm_file}
        COMMAND ${WAT2WASM_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${wat_file} -o ${CMAKE_CURRENT_BINARY_DIR}/${wasm_file}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${wat_file}
        COMMENT "Converting ${wat_file} to ${wasm_file}"
        VERBATIM
    )
    
    list(APPEND WASM_FILES ${CMAKE_CURRENT_BINARY_DIR}/${wasm_file})
endforeach()

# Custom target to build all .wasm files
add_custom_target(wasm_tests ALL DEPENDS ${WASM_FILES})

# Use wacgen and wacgen_rt from parent directory
# Targets are defined in the main CMakeLists.txt

# Test: generate C code from nop_test.wasm
add_custom_target(test_nop_conversion
    COMMAND ${CMAKE_BINARY_DIR}/wacgen -b nop_test_generated ${CMAKE_CURRENT_BINARY_DIR}/nop_test.wasm
    DEPENDS wacgen ${CMAKE_CURRENT_BINARY_DIR}/nop_test.wasm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing nop function conversion"
)

# Test: compile generated C code
add_custom_target(test_nop_compilation
    COMMAND ${CMAKE_C_COMPILER} -I${CMAKE_SOURCE_DIR}/runtime -c nop_test_generated.c -o nop_test_generated.o
    DEPENDS test_nop_conversion
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing compilation of generated C code"
)

# Test: generate C code from control flow tests
add_custom_target(test_if_conversion
    COMMAND ${CMAKE_BINARY_DIR}/wacgen -b if_test_generated ${CMAKE_CURRENT_BINARY_DIR}/if_test.wasm
    DEPENDS wacgen ${CMAKE_CURRENT_BINARY_DIR}/if_test.wasm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing if function conversion"
)

add_custom_target(test_loop_conversion
    COMMAND ${CMAKE_BINARY_DIR}/wacgen -b loop_test_generated ${CMAKE_CURRENT_BINARY_DIR}/loop_test.wasm
    DEPENDS wacgen ${CMAKE_CURRENT_BINARY_DIR}/loop_test.wasm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing loop function conversion"
)

add_custom_target(test_branch_conversion
    COMMAND ${CMAKE_BINARY_DIR}/wacgen -b branch_test_generated ${CMAKE_CURRENT_BINARY_DIR}/branch_test.wasm
    DEPENDS wacgen ${CMAKE_CURRENT_BINARY_DIR}/branch_test.wasm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing branch function conversion"
)

add_custom_target(test_return_conversion
    COMMAND ${CMAKE_BINARY_DIR}/wacgen -b return_test_generated ${CMAKE_CURRENT_BINARY_DIR}/return_test.wasm
    DEPENDS wacgen ${CMAKE_CURRENT_BINARY_DIR}/return_test.wasm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing return function conversion"
)

add_custom_target(test_block_conversion
    COMMAND ${CMAKE_BINARY_DIR}/wacgen -b block_test_generated ${CMAKE_CURRENT_BINARY_DIR}/block_test.wasm
    DEPENDS wacgen ${CMAKE_CURRENT_BINARY_DIR}/block_test.wasm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing block function conversion"
)

# Test: compile generated C code for control flow tests
add_custom_target(test_if_compilation
    COMMAND ${CMAKE_C_COMPILER} -I${CMAKE_SOURCE_DIR}/runtime -c if_test_generated.c -o if_test_generated.o
    DEPENDS test_if_conversion
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing compilation of generated if C code"
)

add_custom_target(test_loop_compilation
    COMMAND ${CMAKE_C_COMPILER} -I${CMAKE_SOURCE_DIR}/runtime -c loop_test_generated.c -o loop_test_generated.o
    DEPENDS test_loop_conversion
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing compilation of generated loop C code"
)

add_custom_target(test_branch_compilation
    COMMAND ${CMAKE_C_COMPILER} -I${CMAKE_SOURCE_DIR}/runtime -c branch_test_generated.c -o branch_test_generated.o
    DEPENDS test_branch_conversion
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing compilation of generated branch C code"
)

add_custom_target(test_return_compilation
    COMMAND ${CMAKE_C_COMPILER} -I${CMAKE_SOURCE_DIR}/runtime -c return_test_generated.c -o return_test_generated.o
    DEPENDS test_return_conversion
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing compilation of generated return C code"
)

add_custom_target(test_block_compilation
    COMMAND ${CMAKE_C_COMPILER} -I${CMAKE_SOURCE_DIR}/runtime -c block_test_generated.c -o block_test_generated.o
    DEPENDS test_block_conversion
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Testing compilation of generated block C code"
)

# Run all tests
add_custom_target(run_tests
    DEPENDS test_nop_compilation test_if_compilation test_loop_compilation test_branch_compilation test_return_compilation test_block_compilation
    COMMENT "All tests completed successfully"
)