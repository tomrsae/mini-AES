cmake_minimum_required(VERSION 3.1)

project(mini-AES)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(miniaes main.c miniaes.c miniaes.h keysched.h keysched.c)
