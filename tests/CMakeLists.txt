# Tests CMakeLists.txt

# Find WABT for wat2wasm conversion
find_program(WAT2WASM_EXECUTABLE wat2wasm PATHS ${CMAKE_SOURCE_DIR}/ext/wabt/bin)

if(NOT WAT2WASM_EXECUTABLE)
    message(FATAL_ERROR "wat2wasm not found. Make sure WABT is built and available")
endif()

# Test cases
set(WAT_FILES
    nop_test.wat
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

# Run all tests
add_custom_target(run_tests
    DEPENDS test_nop_compilation
    COMMENT "All tests completed successfully"
)