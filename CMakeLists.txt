# COPYRIGHT VV.EXE Development Team 2026
cmake_minimum_required(VERSION 3.15)
project(STALZONE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# These modules are unsupported stubs without a documented backend/API.
set(UNSUPPORTED_FEATURE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/auction/scanner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/misc/quick_auction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/players/comparison.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/players/radar.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/misc/bhop.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/misc/panic_key.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/world/item_esp.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/features/world/stash_esp.cpp"
)
list(REMOVE_ITEM SOURCES ${UNSUPPORTED_FEATURE_SOURCES})

add_library(vv SHARED ${SOURCES})

target_include_directories(vv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(vv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_link_libraries(vv PRIVATE user32 gdi32 opengl32)
