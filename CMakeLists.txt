cmake_minimum_required(VERSION 3.11)

project(SECSY)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(SECSY
        src/Test.cpp
)

target_include_directories(SECSY PUBLIC
        include
)

add_executable(main
        src/main.cpp
)

target_link_libraries(main PRIVATE SECSY)