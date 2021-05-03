cmake_minimum_required(VERSION 3.17.0)
project(clox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_BUILD_TYPE Debug)

add_executable(clox "")
add_subdirectory(src)

set(CXX_COMPILE_FLAGS)
set(CXX_LINK_FLAGS)

list(APPEND CXX_FLAGS -Wall -Wextra -pedantic)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND CXX_COMPILE_FLAGS -fno-omit-frame-pointer
            -fno-optimize-sibling-calls -fsanitize=address)
    list(APPEND CXX_LINK_FLAGS -fsanitize=address -fno-omit-frame-pointer
            -fno-optimize-sibling-calls)
    target_compile_definitions(clox PRIVATE DEBUG_TRACE_EXECUTION)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    list(APPEND CXX_COMPILE_FLAGS -O3)
endif()

target_compile_options(clox PRIVATE ${CXX_COMPILE_FLAGS})
target_link_options(clox PRIVATE ${CXX_LINK_FLAGS})
target_include_directories(clox PRIVATE SYSTEM "include")
