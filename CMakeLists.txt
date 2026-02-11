cmake_minimum_required(VERSION 3.20)
project(wacgen VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add subdirectories for dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ext/binaryen)
    add_subdirectory(ext/binaryen)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ext/wabt)
    add_subdirectory(ext/wabt)
endif()

# Main wacgen executable
add_executable(wacgen)
target_sources(wacgen PRIVATE
    src/main.c
    src/wacgen_core.c
    src/codegen/codegen.c
)

target_include_directories(wacgen PRIVATE
    src
    src/codegen
)

# Link with Binaryen
if(TARGET binaryen)
    target_link_libraries(wacgen PRIVATE binaryen)
endif()

# Runtime library
add_library(wacgen_rt SHARED)
target_sources(wacgen_rt PRIVATE
    runtime/wacgen_rt.c
)

target_include_directories(wacgen_rt PUBLIC
    runtime
)

# Tests
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS wacgen DESTINATION bin)
install(TARGETS wacgen_rt DESTINATION lib)