cmake_minimum_required(VERSION 3.15)

# Find ctransian library
if(NOT TARGET ctransian-core)
    find_package(ctransian REQUIRED)
endif()

# Simple math example
add_executable(example_math simple_math.c)
target_link_libraries(example_math ctransian-runtime)

# Factorial example (when available)
# add_executable(example factorial factorial.c)
# target_link_libraries(factorial ctransian-runtime)