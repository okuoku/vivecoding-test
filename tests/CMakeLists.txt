# Test CMakeLists.txt for wacgen
# Assembles .wat files to .wasm using WABT's wasm-as

# Find WABT tools (will be built as dependency)
find_program(WASM_AS_EXECUTABLE wasm-as PATHS ${CMAKE_BINARY_DIR}/ext/wabt NO_DEFAULT_PATH)

# Collect all .wat files
file(GLOB_RECURSE WAT_FILES "*.wat")

# Add dependency on WABT tools
if(TARGET wabt::wasm-as)
    set(WASM_AS_EXECUTABLE $<TARGET_FILE:wabt::wasm-as>)
else()
    # Create a dummy target if wabt not available
    add_custom_target(wabt_tools)
endif()

# Create .wasm files from .wat files
foreach(WAT_FILE ${WAT_FILES})
    get_filename_component(FILE_NAME ${WAT_FILE} NAME_WE)
    get_filename_component(FILE_DIR ${WAT_FILE} DIRECTORY)
    set(WASM_FILE ${FILE_DIR}/${FILE_NAME}.wasm)
    
    if(TARGET wabt::wasm-as)
        add_custom_command(
            OUTPUT ${WASM_FILE}
            COMMAND ${WASM_AS_EXECUTABLE} ${WAT_FILE} -o ${WASM_FILE}
            DEPENDS ${WAT_FILE} wabt::wasm-as
            COMMENT "Assembling ${WAT_FILE} to ${WASM_FILE}"
            VERBATIM
        )
    else()
        add_custom_command(
            OUTPUT ${WASM_FILE}
            COMMAND echo "wasm-as not available, skipping ${WAT_FILE}"
            DEPENDS ${WAT_FILE}
            COMMENT "Skipping ${WAT_FILE} - WABT not available"
        )
    endif()
    
    list(APPEND WASM_FILES ${WASM_FILE})
endforeach()

# Custom target to build all test files
add_custom_target(test_files ALL DEPENDS ${WASM_FILES})

# Add test for nop conversion
set(NOP_WASM ${CMAKE_CURRENT_SOURCE_DIR}/nop.wasm)
set(NOP_OUTPUT_BASE ${CMAKE_CURRENT_BINARY_DIR}/nop)

# Test case: convert nop.wasm to C source
add_test(NAME nop_conversion 
    COMMAND wacgen -b nop -o ${NOP_OUTPUT_BASE}.c ${NOP_WASM}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Test compilation of generated C code
add_test(NAME nop_compilation
    COMMAND ${CMAKE_C_COMPILER} -c ${NOP_OUTPUT_BASE}.c -o ${NOP_OUTPUT_BASE}.o -I${CMAKE_SOURCE_DIR}/runtime
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
