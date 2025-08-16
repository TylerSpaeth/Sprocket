cmake_minimum_required(VERSION 3.28.2)

project(SprocketApp)

add_executable(app
    src/TestApplication.cpp
)

target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/Sprocket/include
    ${CMAKE_SOURCE_DIR}/Sprocket/src
)

# Include everything in the resources folder
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Example/res
        $<TARGET_FILE_DIR:app>
)

target_link_libraries(app PRIVATE sprocket)