cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(PaintGame VERSION 0.0.1)

file(GLOB Sources "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(PaintGame ${Sources})

target_include_directories(PaintGame PUBLIC "${PROJECT_SOURCE_DIR}/vendor/spdlog/include")