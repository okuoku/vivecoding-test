cmake_minimum_required(VERSION 3.15)
project(ctransian VERSION 1.0.0 LANGUAGES C CXX)

# C++ standard for core components
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C standard for generated output and runtime
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-error=extra-semi -Wno-error=format-pedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
endif()

# Add vendored Binaryen
add_subdirectory(third_party/binaryen)

# Optional: Find Threads
find_package(Threads)

# Project options
option(CTRANIAN_ENABLE_WASI "Enable WASI support" ON)
option(CTRANIAN_ENABLE_GC "Enable WasmGC support" OFF)
option(CTRANIAN_BUILD_TESTS "Build tests" ON)
option(CTRANIAN_BUILD_EXAMPLES "Build examples" ON)
option(CTRANIAN_ENABLE_SIMD "Enable SIMD support" ON)
option(CTRANIAN_ENABLE_THREADS "Enable threading support" ON)

# Configuration
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ctransian/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/ctransian/config.h"
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/binaryen/src
)

# Core library sources
set(CORE_SOURCES
    src/core/translator.cpp
    src/core/type_mapper.cpp
    src/core/instruction_visitor.cpp
    src/core/code_generator.cpp
    src/api/ctransian.cpp
    src/runtime/ctransian_runtime.c
)

# Runtime sources
set(RUNTIME_SOURCES
    src/runtime/memory.c
    src/runtime/threads.c
    src/runtime/exceptions.c
    src/runtime/imports.c
)

# Create core library
add_library(ctransian-core STATIC ${CORE_SOURCES})
target_include_directories(ctransian-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)
target_link_libraries(ctransian-core
    PRIVATE
        binaryen
        $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:Threads::Threads>
)

# Create shared library
add_library(ctransian-shared SHARED ${CORE_SOURCES})
target_include_directories(ctransian-shared
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)
target_link_libraries(ctransian-shared
    PRIVATE
        binaryen
        $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:Threads::Threads>
)
set_target_properties(ctransian-shared PROPERTIES OUTPUT_NAME ctransian)

# CLI executable
add_executable(ctransian src/cli/main.cpp)
target_link_libraries(ctransian ctransian-core)
set_target_properties(ctransian PROPERTIES OUTPUT_NAME ctransian)

# Runtime library (for generated code)
add_library(ctransian-runtime STATIC ${RUNTIME_SOURCES})
target_include_directories(ctransian-runtime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(ctransian-runtime
    PUBLIC
        $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:Threads::Threads>
)

# Conditional features
if(CTRANIAN_ENABLE_WASI)
    add_compile_definitions(CTRANIAN_ENABLE_WASI=1)
    list(APPEND CORE_SOURCES src/wasi/wasi_impl.cpp)
endif()

if(CTRANIAN_ENABLE_GC)
    add_compile_definitions(CTRANIAN_ENABLE_GC=1)
    list(APPEND CORE_SOURCES src/gc/gc_impl.cpp)
endif()

if(CTRANIAN_ENABLE_SIMD)
    add_compile_definitions(CTRANIAN_ENABLE_SIMD=1)
endif()

if(CTRANIAN_ENABLE_THREADS)
    add_compile_definitions(CTRANIAN_ENABLE_THREADS=1)
endif()

# Testing
if(CTRANIAN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples (disabled during development)
# if(CTRANIAN_BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()

# Installation (commented out during development)
# include(GNUInstallDirs)

# install(TARGETS ctransian-core ctransian-shared ctransian-runtime ctransian
#     EXPORT CtransianTargets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(DIRECTORY include/ctransian
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#     FILES_MATCHING PATTERN "*.h"
# )

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/ctransian/config.h
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ctransian
# )

# Export configuration (commented out during development)
# install(EXPORT CtransianTargets
#     FILE CtransianTargets.cmake
#     NAMESPACE Ctransian::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Ctransian
# )

# Generate config file (commented out during development)
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CtransianConfig.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/CtransianConfig.cmake"
#     INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Ctransian
# )

# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/CtransianConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY SameMajorVersion
# )

# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/CtransianConfig.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/CtransianConfigVersion.cmake"
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Ctransian
# )

# Development targets
add_custom_target(run-tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS ctest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(format
    COMMAND find src include tests -name "*.cpp" -o -name "*.c" -o -name "*.h" | xargs clang-format -i
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(lint
    COMMAND cpplint --recursive src include tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Print configuration summary
message(STATUS "")
message(STATUS "ctransian configuration summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  C standard: ${CMAKE_C_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Features:")
message(STATUS "  WASI support: ${CTRANIAN_ENABLE_WASI}")
message(STATUS "  WasmGC support: ${CTRANIAN_ENABLE_GC}")
message(STATUS "  SIMD support: ${CTRANIAN_ENABLE_SIMD}")
message(STATUS "  Threading support: ${CTRANIAN_ENABLE_THREADS}")
message(STATUS "  Build tests: ${CTRANIAN_BUILD_TESTS}")
message(STATUS "  Build examples: ${CTRANIAN_BUILD_EXAMPLES}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  Binaryen: vendored (third_party/binaryen)")
message(STATUS "  Threads available: ${CMAKE_USE_PTHREADS_INIT}")
message(STATUS "")