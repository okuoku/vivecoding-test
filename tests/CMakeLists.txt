cmake_minimum_required(VERSION 3.15)

# Basic test configuration
enable_testing()

# Find Binaryen tools for WAT → WASM conversion
find_program(WASM_AS_TOOL 
    NAMES wasm-as
    PATHS ${CMAKE_BINARY_DIR}/third_party/binaryen/bin
    DOC "Binaryen wasm-as tool for converting .wat to .wasm"
)

find_program(WASM_DIS_TOOL 
    NAMES wasm-dis
    PATHS ${CMAKE_BINARY_DIR}/third_party/binaryen/bin
    DOC "Binaryen wasm-dis tool for disassembling .wasm to .wat"
)

find_program(WASM_SHELL_TOOL 
    NAMES wasm-shell
    PATHS ${CMAKE_BINARY_DIR}/third_party/binaryen/bin
    DOC "Binaryen wasm-shell tool for executing .wasm files"
)

# Report tool availability
if(NOT WASM_AS_TOOL)
    message(WARNING "wasm-as tool not found - WAT → WASM conversion tests will be skipped")
endif()

if(NOT WASM_DIS_TOOL)
    message(WARNING "wasm-dis tool not found - WASM validation tests will be skipped")
endif()

if(NOT WASM_SHELL_TOOL)
    message(WARNING "wasm-shell tool not found - WASM execution tests will be skipped")
endif()

# Test utilities library
add_library(test_utils STATIC 
    utils/test_helpers.cpp
    utils/test_helpers.h
)
target_include_directories(test_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)

# Basic test executable
add_executable(test_ctransian_basic test_basic.cpp)
target_link_libraries(test_ctransian_basic ctransian-core)

# Constants test executable
add_executable(test_ctransian_constants test_constants.cpp)
target_link_libraries(test_ctransian_constants ctransian-core)

# Phase 0 integration test executable
add_executable(test_phase0_integration integration/test_phase0_integration.cpp)
target_link_libraries(test_phase0_integration ctransian-core test_utils)

# Phase 1 parametric test executable
add_executable(test_phase1_parametric unit/test_parametric.cpp)
target_link_libraries(test_phase1_parametric ctransian-core test_utils)

# Phase 1 variables test executable (TODO: create test_variables.cpp)
# add_executable(test_phase1_variables unit/test_variables.cpp)
# target_link_libraries(test_phase1_variables ctransian-core test_utils)

# Phase 1 control flow test executable (TODO: create test_control_flow.cpp)
# add_executable(test_phase1_control_flow unit/test_control_flow.cpp)
# target_link_libraries(test_phase1_control_flow ctransian-core test_utils)

# Add tests to CTest
add_test(NAME BasicTest COMMAND test_ctransian_basic)
add_test(NAME ConstantsTest COMMAND test_ctransian_constants)

# Add Phase 0 tests if tools are available
if(WASM_AS_TOOL)
    add_test(NAME Phase0IntegrationTest COMMAND test_phase0_integration)
endif()

# Add Phase 1 tests if tools are available
if(WASM_AS_TOOL)
    add_test(NAME Phase1ParametricTest COMMAND test_phase1_parametric)
    # add_test(NAME Phase1VariablesTest COMMAND test_phase1_variables)
    # add_test(NAME Phase1ControlFlowTest COMMAND test_phase1_control_flow)
endif()

# Custom targets for generating test WASM files from WAT files
if(WASM_AS_TOOL)
    # Phase 0 constants test files
    set(PHASE0_WAT_FILES
        test_data/phase0/constants/simple_constants.wat
        test_data/phase0/constants/special_floats.wat
        test_data/phase0/constants/extreme_values.wat
    )
    
    foreach(WAT_FILE ${PHASE0_WAT_FILES})
        get_filename_component(WAT_NAME ${WAT_FILE} NAME_WE)
        get_filename_component(WAT_DIR ${WAT_FILE} DIRECTORY)
        set(WASM_FILE ${WAT_DIR}/${WAT_NAME}.wasm)
        
        add_custom_command(
            OUTPUT ${WASM_FILE}
            COMMAND ${WASM_AS_TOOL} ${CMAKE_CURRENT_SOURCE_DIR}/${WAT_FILE} -o ${WASM_FILE}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${WAT_FILE}
            COMMENT "Converting ${WAT_FILE} to ${WASM_FILE}"
            VERBATIM
        )
        
        list(APPEND PHASE0_WASM_FILES ${WASM_FILE})
    endforeach()
    
    # Phase 1 test files
    set(PHASE1_WAT_FILES
        test_data/phase1/parametric/parametric_tests.wat
        test_data/phase1/variables/variables_tests.wat
        test_data/phase1/control_flow/control_flow_tests.wat
    )
    
    foreach(WAT_FILE ${PHASE1_WAT_FILES})
        get_filename_component(WAT_NAME ${WAT_FILE} NAME_WE)
        get_filename_component(WAT_DIR ${WAT_FILE} DIRECTORY)
        set(WASM_FILE ${WAT_DIR}/${WAT_NAME}.wasm)
        
        add_custom_command(
            OUTPUT ${WASM_FILE}
            COMMAND ${WASM_AS_TOOL} ${CMAKE_CURRENT_SOURCE_DIR}/${WAT_FILE} -o ${WASM_FILE}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${WAT_FILE}
            COMMENT "Converting ${WAT_FILE} to ${WASM_FILE}"
            VERBATIM
        )
        
        list(APPEND PHASE1_WASM_FILES ${WASM_FILE})
    endforeach()
    
    # Custom targets for generating all test WASM files
    add_custom_target(generate_phase0_wasm ALL DEPENDS ${PHASE0_WASM_FILES})
    add_custom_target(generate_phase1_wasm ALL DEPENDS ${PHASE1_WASM_FILES})
    
    # Make test executables depend on WASM file generation
    if(TARGET test_phase0_integration)
        add_dependencies(test_phase0_integration generate_phase0_wasm)
    endif()
    
    if(TARGET test_phase1_parametric)
        add_dependencies(test_phase1_parametric generate_phase1_wasm)
    endif()
    
    # if(TARGET test_phase1_variables)
#     add_dependencies(test_phase1_variables generate_phase1_wasm)
# endif()
    
# if(TARGET test_phase1_control_flow)
#     add_dependencies(test_phase1_control_flow generate_phase1_wasm)
# endif()
endif()