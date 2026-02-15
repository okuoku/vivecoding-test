execute_process(
    COMMAND ${CMAKE_COMMAND}
    -S dummy_test
    -B __dummy
    -G Ninja)
