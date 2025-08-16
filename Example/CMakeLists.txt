cmake_minimum_required(VERSION 3.28.2)
set(CMAKE_CXX_STANDARD 20)

# Specifies the name of the project
project(app)

# Compiles the Sprocket dll
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Sprocket ${CMAKE_CURRENT_SOURCE_DIR}/../Sprocket/build )

#add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(${PROJECT_NAME} src/TestApplication.cpp)

# Include the public headers from Sprocket
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Sprocket/include)

# Links the sprocket library to the target
target_link_libraries(${PROJECT_NAME} PRIVATE sprocket)

# Copys the Sprocket dll into the build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/../Sprocket/build/libsprocket.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Copys the Default.vert shader into the build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/../Sprocket/res/shaders/Default.vert"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Copys the Default.frag shader into the build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/../Sprocket/res/shaders/Default.frag"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Copys the Default.frag shader into the build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/../Sprocket/res/shaders/Default32.frag"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

