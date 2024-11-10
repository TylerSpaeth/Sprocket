cmake_minimum_required(VERSION 3.28.2)

project(SprocketTest)

# Specify that these are tests so dllexport and dllimport is not used
add_compile_definitions(${PROJECT_NAME} PRIVATE SPROCKET_UNIT_TEST)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Test sources
target_sources(${PROJECT_NAME} PRIVATE src/EventTests.cpp src/CollisionTests.cpp src/InputTests.cpp)

# Sprocket Sources
target_sources(${PROJECT_NAME} PRIVATE ../src/ECS/Collision.cpp ../src/Core/Input.cpp)

target_link_libraries(${PROJECT_NAME} GTest::gtest_main GTest::gmock_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})