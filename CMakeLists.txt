cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(PaintGame VERSION 0.0.1)

file(GLOB Sources "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB imgui "${PROJECT_SOURCE_DIR}/vendor/imgui/*.cpp")

add_executable(PaintGame ${Sources} ${imgui})

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(PaintGame ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

find_package(GLEW REQUIRED)
target_link_libraries(PaintGame GLEW::GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(PaintGame OpenGL::OpenGL)

target_include_directories(PaintGame PUBLIC "${PROJECT_SOURCE_DIR}/vendor/spdlog/include")
target_include_directories(PaintGame PUBLIC "${PROJECT_SOURCE_DIR}/vendor/imgui")

add_custom_target(copy_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
)
add_dependencies(PaintGame copy_data)