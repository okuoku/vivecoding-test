cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(test C)

add_executable(test_exec
    main.c
    obj.c)
