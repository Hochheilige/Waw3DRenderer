list(
    APPEND WawMathSrc
    FPS.cpp FPS.h
    mat4x4.cpp mat4x4.h
    MathTriangle.h
    Mesh.cpp Mesh.h 
    vec3d.cpp vec3d.h 
    WawMath.h
)

message("-- build source files")
add_library(WawMathSources STATIC ${WawMathSrc})

target_include_directories(WawMathSources
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/include/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/src/ 
)

target_link_directories(WawMathSources
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/include/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/WinAPIWrapper/src/ 
)