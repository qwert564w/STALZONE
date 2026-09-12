# COPYRIGHT VV.EXE Development Team 2026
cmake_minimum_required(VERSION 3.15)
project(STALZONE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(vv SHARED ${SOURCES})

target_include_directories(vv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(vv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

# Link necessary libraries
target_link_libraries(vv PRIVATE user32 gdi32 opengl32)
