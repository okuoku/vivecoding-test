cmake_minimum_required(VERSION 3.20)
project(wacgen VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add Binaryen
add_subdirectory(ext/binaryen)

# Add WABT without tests
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(ext/wabt)

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
    ext/binaryen/src
)

# Link with Binaryen
target_link_libraries(wacgen PRIVATE binaryen)

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
