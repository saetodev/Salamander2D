cmake_minimum_required(VERSION 3.30)

project(Salamander2D)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("vendor/glfw")

file(GLOB SRC_FILES "src/*.cpp")

add_library(Salamander2D STATIC ${SRC_FILES})

target_include_directories(Salamander2D PUBLIC "include" "vendor/glfw/include" "vendor/stb")
target_link_libraries(Salamander2D "glfw" "GL")