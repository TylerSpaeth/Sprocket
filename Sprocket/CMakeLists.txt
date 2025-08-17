cmake_minimum_required(VERSION 3.28.2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SprocketApp)

add_compile_definitions(sprocket PRIVATE SPROCKET_BUILD_DLL)
link_directories(lib)

add_library(sprocket SHARED)

target_include_directories(sprocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(sprocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_precompile_headers(sprocket PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Sprocket.pch>)

# Add ThirdParty source code
target_sources(sprocket PRIVATE 
  src/ThirdParty/glad/glad.c
  src/ThirdParty/stb_image/stb_image.cpp
)

# Add Renderer source code
target_sources(sprocket PRIVATE 
  src/Renderer/VertexBuffer.cpp 
  src/Renderer/IndexBuffer.cpp 
  src/Renderer/VertexArray.cpp 
  src/Renderer/Renderer.cpp 
  src/Renderer/Texture.cpp 
  src/Renderer/Shader.cpp
)

# Add Core source code
target_sources(sprocket PRIVATE 
  src/Core/Window.cpp
  src/Core/Application.cpp
  src/Core/Input.cpp
  src/Core/Global.cpp
)

# Add Scene source code
target_sources(sprocket PRIVATE 
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
target_sources(sprocket PRIVATE
  src/Physics/Physics.cpp
  src/Physics/Collision.cpp
)

# Add Events source code
target_sources(sprocket PRIVATE
  src/Events/EventValidation.cpp
)

# Add Logger source code
target_sources(sprocket PRIVATE
  src/Logger/Logger.cpp
)

# Copy everything in the resources folder to the output directory
add_custom_command(TARGET sprocket POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:sprocket>/
)

target_link_libraries(sprocket PRIVATE glfw3)