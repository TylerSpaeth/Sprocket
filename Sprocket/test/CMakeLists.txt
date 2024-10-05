cmake_minimum_required(VERSION 3.28.2)

project(SprocketTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME})

# Test sources
target_sources(${PROJECT_NAME} PRIVATE src/EventTests.cpp)

# Sprocket Sources
target_include_directories(${PROJECT_NAME} PRIVATE ../src/Events)
target_sources(${PROJECT_NAME} PRIVATE ../src/Events/)


target_link_libraries(${PROJECT_NAME} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})