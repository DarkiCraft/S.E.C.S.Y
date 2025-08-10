add_library(SECSY INTERFACE)

target_include_directories(SECSY INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(SECSY INTERFACE
        raylib
        raylib_cpp
)


if(APPLE)
    target_link_libraries(SECSY INTERFACE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
elseif(UNIX)
    target_link_libraries(SECSY INTERFACE
        m
        pthread
        dl
        rt
        X11
    )
elseif(WIN32)
    target_link_libraries(SECSY INTERFACE
        winmm
        gdi32
    )
endif()
