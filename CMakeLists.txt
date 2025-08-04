cmake_minimum_required(VERSION 3.11)

project(SECSY)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

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

set(FETCHCONTENT_QUIET OFF)
set(BUILD_SHARED_LIBS OFF)
set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

add_library(${PROJECT_NAME}
    src/Test.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    raylib_cpp
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        m
        pthread
        dl
        rt
        X11
    )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        winmm
        gdi32
    )
endif()


add_executable(main
    src/main.cpp
)

target_link_libraries(main PRIVATE ${PROJECT_NAME})