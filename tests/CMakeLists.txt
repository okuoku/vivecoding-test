cmake_minimum_required(VERSION 3.15)

# Basic test configuration
enable_testing()

# Find ctransian library
if(NOT TARGET ctransian-core)
    find_package(ctransian REQUIRED)
endif()

# Test executable
add_executable(test_ctransian_basic test_basic.cpp)
target_link_libraries(test_ctransian_basic ctransian-core)

# Add test to CTest
add_test(NAME BasicTest COMMAND test_ctransian_basic)

# Test with sample WASM file
add_executable(test_ctransian_sample test_sample.cpp)
target_link_libraries(test_ctransian_sample ctransian-core)

add_test(NAME SampleTest COMMAND test_ctransian_sample)

# Performance test
add_executable(test_ctransian_performance test_performance.cpp)
target_link_libraries(test_ctransian_performance ctransian-core)

add_test(NAME PerformanceTest COMMAND test_ctransian_performance)

# Integration test
add_executable(test_ctransian_integration test_integration.cpp)
target_link_libraries(test_ctransian_integration ctransian-core)

add_test(NAME IntegrationTest COMMAND test_ctransian_integration)