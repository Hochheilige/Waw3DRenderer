cmake_minimum_required(VERSION 3.15)

message("-- Build Waw3DRenderer")
project(Waw3DRenderer LANGUAGES CXX)

add_subdirectory(external/WinAPIWrapper)

add_executable(${PROJECT_NAME} WIN32 Application.cpp)

    target_include_directories(${PROJECT_NAME}
	    PRIVATE external/WinAPIWrapper/src/ 
    )

    target_link_directories(${PROJECT_NAME} 
	    PRIVATE external/WinAPIWrapper/src/
            PRIVATE models/
    )

target_link_libraries(${PROJECT_NAME} WinAPIWrapper)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
