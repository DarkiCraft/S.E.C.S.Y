cmake_minimum_required(VERSION 3.14)
project(FlatSetTests LANGUAGES CXX)

# Require C++17 (needed for structured bindings, etc.)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable CTest
enable_testing()

# Test executable
add_executable(test_flatset
    test_flatset.cpp
)

# Include directory for FlatSet.hpp
# Assuming FlatSet.hpp is in the same directory or an "include" folder
target_include_directories(test_flatset
    PRIVATE
        ${CMAKE_SOURCE_DIR}         # if FlatSet.hpp is in project root
        ${CMAKE_SOURCE_DIR}/include # if stored in include/
)

# Add test to CTest
add_test(NAME FlatSetTests COMMAND test_flatset)

# Optional: more strict compiler warnings
if (MSVC)
    target_compile_options(test_flatset PRIVATE /W4 /permissive-)
else()
    target_compile_options(test_flatset PRIVATE -Wall -Wextra -Wpedantic)
endif()
