cmake_minimum_required(VERSION 3.28.2)

# Specifies the name of the project
project(myproject)

# Specifies where our include directory is
include_directories(src)

# Links our lib directory to be used
link_directories(lib)

# Specifies that we want to create an executable using certain source code
add_executable(${PROJECT_NAME} src/main.cpp)

# Add ThirdParty source code
target_sources(${PROJECT_NAME} PRIVATE 
  src/ThirdParty/glad/glad.c
  src/ThirdParty/stb_image/stb_image.cpp
)

# Add Renderer source code
target_sources(${PROJECT_NAME} PRIVATE 
  src/Renderer/VertexBuffer.cpp 
  src/Renderer/IndexBuffer.cpp 
  src/Renderer/VertexArray.cpp 
  src/Renderer/Renderer.cpp 
  src/Renderer/Texture.cpp 
  src/Renderer/Shader.cpp
)

# Add source code
target_sources(${PROJECT_NAME} PRIVATE 
  src/Input.cpp
)

# Links libraries in the lib folder to the executable
target_link_libraries(${PROJECT_NAME} libglfw3.a)

# This increases the stack size to 10mb, which is necessary based on how the 
# renderer portion currently works. In reality, those sections that take up large
# amounts of memory should be on the heap
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000") 

