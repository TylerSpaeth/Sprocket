cmake_minimum_required(VERSION 3.28.2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SprocketApp)

add_compile_definitions(sprocket PRIVATE SPROCKET_BUILD_DLL)
link_directories(lib)

add_library(sprocket SHARED)

# Expose the raw ImGui API
target_compile_definitions(sprocket PRIVATE "IMGUI_API=__declspec(dllexport)")
target_compile_definitions(sprocket INTERFACE "IMGUI_API=__declspec(dllimport)")

target_include_directories(sprocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(sprocket PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(sprocket PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty)
target_include_directories(sprocket PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/imgui-docking)
target_include_directories(sprocket PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/imgui-docking/backends)
target_include_directories(sprocket PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/freetype)

target_precompile_headers(sprocket PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Sprocket.pch>)

# Add ThirdParty source code
target_sources(sprocket PRIVATE
  src/ThirdParty/glad/glad.c
  src/ThirdParty/stb_image/stb_image.cpp
  src/ThirdParty/imgui-docking/imgui.cpp
  src/ThirdParty/imgui-docking/imgui_demo.cpp
  src/ThirdParty/imgui-docking/imgui_draw.cpp
  src/ThirdParty/imgui-docking/imgui_tables.cpp
  src/ThirdParty/imgui-docking/imgui_widgets.cpp
  src/ThirdParty/imgui-docking/backends/imgui_impl_glfw.cpp
  src/ThirdParty/imgui-docking/backends/imgui_impl_opengl3.cpp
)

# Add Renderer source code
target_sources(sprocket PRIVATE 
  src/Renderer/VertexBuffer.cpp 
  src/Renderer/IndexBuffer.cpp 
  src/Renderer/VertexArray.cpp 
  src/Renderer/Renderer.cpp 
  src/Renderer/Texture.cpp 
  src/Renderer/Shader.cpp
  src/Renderer/Font.cpp
)

# Add Core source code
target_sources(sprocket PRIVATE 
  src/Core/Window.cpp
  src/Core/Application.cpp
  src/Core/Input.cpp
)

# Add Scene source code
target_sources(sprocket PRIVATE 
  src/Scene/SceneManager.cpp
  src/Scene/Scene.cpp
  src/Scene/Entity.cpp
  src/Scene/Components/TransformComponent.cpp
  src/Scene/Components/QuadRendererComponent.cpp
  src/Scene/Components/CameraComponent.cpp
  src/Scene/Components/ColliderComponent.cpp
  src/Scene/Components/BoxColliderComponent.cpp
  src/Scene/Components/CircleColliderComponent.cpp
  src/Scene/Components/TileMapComponent.cpp
  src/Scene/Components/SoundComponent.cpp
  src/Scene/Components/AnimationComponent.cpp
  src/Scene/Components/TextRendererComponent.cpp
)

# Add Physics source code
target_sources(sprocket PRIVATE
  src/Physics/PhysicsManager.cpp
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

# Add ImGui source code
target_sources(sprocket PRIVATE
  src/ImGui/ImGuiImpl.cpp
)

# Add Audio source code
target_sources(sprocket PRIVATE
  src/Audio/AudioManager.cpp
  src/Audio/Sound.cpp
)

# Copy everything in the resources folder to the output directory
add_custom_command(TARGET sprocket POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:sprocket>/
)

target_link_libraries(sprocket PRIVATE glfw3)
target_link_libraries(sprocket PRIVATE freetype)