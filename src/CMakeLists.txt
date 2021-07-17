list(
    APPEND src
    FPS.cpp FPS.h
    Mesh.cpp Mesh.h 
)

add_subdirectory(math)

message("-- build source files")
add_library(sources STATIC ${src})

target_include_directories(sources
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/include/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/src/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/math/ 
)

target_link_directories(sources
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/include/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/src/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/math/
)