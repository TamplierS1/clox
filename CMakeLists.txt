cmake_minimum_required(VERSION 3.17.0)
project(clox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_BUILD_TYPE Debug)

add_executable(clox "")
add_subdirectory(src)

target_compile_options(clox PRIVATE -Wall -Wextra -pedantic)

target_include_directories(clox PRIVATE SYSTEM "include")