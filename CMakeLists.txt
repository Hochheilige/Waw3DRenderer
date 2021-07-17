cmake_minimum_required(VERSION 3.15)

option(MANDO_HELMET "Build example with rounded Mando helmet" OFF)
option(SPACESHIP "Build example with rounded Spaceship" OFF)
set(BUILD_AS_STATIC_LIB ON)

message("-- Build Waw3DRenderer")
project(Waw3DRenderer LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(external/WinAPIWrapper)

if (MANDO_HELMET OR SPACESHIP)
    message("-- Build example program")
    add_subdirectory(examples)
else()
    message("-- Build app program")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/axis.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/models)
    add_executable(${PROJECT_NAME} WIN32 Application.cpp)

    target_include_directories(${PROJECT_NAME}
        PRIVATE external/WinAPIWrapper/src/ 
        PRIVATE src/ 
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/WinAPIWrapper/include/ 
    )

    target_link_directories(${PROJECT_NAME} 
        PRIVATE external/WinAPIWrapper/src/
        PRIVATE src/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/WinAPIWrapper/include/ 
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/models/
    )
endif()

target_link_libraries(${PROJECT_NAME} WinAPIWrapper mathLib sources)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
