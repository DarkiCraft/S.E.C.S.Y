cmake_minimum_required(VERSION 3.11)
project(SECSY LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

include(CTest)
include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG        8e01c1dd8fbbe12199fc1256c3bc801d5745ad48
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib_cpp)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(tests)
