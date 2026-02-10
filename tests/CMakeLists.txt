cmake_minimum_required(VERSION 3.15)

# Basic test configuration
enable_testing()

# Test executable
add_executable(test_ctransian_basic test_basic.cpp)
target_link_libraries(test_ctransian_basic ctransian-core)

# Add test to CTest
add_test(NAME BasicTest COMMAND test_ctransian_basic)