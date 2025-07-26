cmake_minimum_required(VERSION 3.28.2)

# Specifies the name of the project
project(sprocket)

# Links our lib directory to be used
link_directories(lib)

add_compile_definitions(${PROJECT_NAME} PRIVATE SPROCKET_BUILD_DLL)

# Specifies that we want to create an executable using certain source code
add_library(${PROJECT_NAME} SHARED)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )

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

# Add Core source code
target_sources(${PROJECT_NAME} PRIVATE 
  src/Core/Window.cpp
  src/Core/Application.cpp
  src/Core/Input.cpp
)

# Add Scene source code
target_sources(${PROJECT_NAME} PRIVATE 
  src/Scene/SceneManager.cpp
  src/Scene/Scene.cpp
  src/Scene/Entity.cpp
  src/Scene/TransformComponent.cpp
  src/Scene/QuadRendererComponent.cpp
  src/Scene/CameraComponent.cpp
  src/Scene/ColliderComponent.cpp
  src/Scene/BoxColliderComponent.cpp
  src/Scene/CircleColliderComponent.cpp
  src/Scene/TileMapComponent.cpp
)

# Add Physics source code
target_sources(${PROJECT_NAME} PRIVATE
  src/Physics/Physics.cpp
  src/Physics/Collision.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Links libraries in the lib folder to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE libglfw3.a)