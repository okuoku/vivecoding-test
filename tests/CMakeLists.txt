cmake_minimum_required(VERSION 3.15)

# Basic test configuration
enable_testing()

# Basic test executable
add_executable(test_ctransian_basic test_basic.cpp)
target_link_libraries(test_ctransian_basic ctransian-core)

# Constants test executable
add_executable(test_ctransian_constants test_constants.cpp)
target_link_libraries(test_ctransian_constants ctransian-core)

# Add tests to CTest
add_test(NAME BasicTest COMMAND test_ctransian_basic)
add_test(NAME ConstantsTest COMMAND test_ctransian_constants)