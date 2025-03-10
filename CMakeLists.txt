cmake_minimum_required(VERSION 3.30)

project(Sal2D)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC_FILES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} "vendor/glad/src/glad.c")

target_include_directories(${PROJECT_NAME} PUBLIC "include" "vendor/glad/include" "vendor/glm" "vendor/stb")
target_link_libraries(${PROJECT_NAME} "glfw" "GL")