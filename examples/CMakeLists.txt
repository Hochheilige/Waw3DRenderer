if (SPACESHIP)
    message("-- Spaceship")
    file(COPY ${CMAKE_SOURCE_DIR}/models/ship.obj DESTINATION ${CMAKE_BINARY_DIR}/models)
    add_executable(${PROJECT_NAME} WIN32 Spaceship.cpp)

    target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/src/ 
        PRIVATE ${CMAKE_SOURCE_DIR}/src/ 
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/include/ 
    )

    target_link_directories(${PROJECT_NAME} 
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/src/
        PRIVATE ${CMAKE_SOURCE_DIR}/src/
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/include/ 
        PRIVATE ${CMAKE_SOURCE_DIR}/models/
    )
elseif(MANDO_HELMET)
    message("-- Mando Helmet")
    file(COPY ${CMAKE_SOURCE_DIR}/models/MandoHelmetTri.obj DESTINATION ${CMAKE_BINARY_DIR}/models)
    add_executable(${PROJECT_NAME} WIN32 MandoHelmet.cpp)

    target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/src/ 
        PRIVATE ${CMAKE_SOURCE_DIR}/src/ 
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/include/ 
    )

    target_link_directories(${PROJECT_NAME} 
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/src/
        PRIVATE ${CMAKE_SOURCE_DIR}/src/
        PRIVATE ${CMAKE_SOURCE_DIR}/external/WinAPIWrapper/include/ 
        PRIVATE ${CMAKE_SOURCE_DIR}/models/
    )
endif()