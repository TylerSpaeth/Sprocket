cmake_minimum_required(VERSION 3.28.2)

# Specifies the name of the project
project(app)

find_library(Sprocketlib Sprocket REQUIRED PATHS ../Sprocket/build)

add_executable(${PROJECT_NAME} src/main.cpp)

# Include the public headers from Sprocket
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Sprocket/include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Sprocketlib})

# This increases the stack size to 10mb, which is necessary based on how the 
# renderer portion currently works. In reality, those sections that take up large
# amounts of memory should be on the heap
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000") 

