cmake_minimum_required(VERSION 3.22.1)
include(FetchContent)

# CMake variables: https://iamsorush.com/posts/cpp-cmake-source-binary-dir/

project(dEngine LANGUAGES CXX)

add_subdirectory(extern/glad)
# add_subdirectory(extern/glfw)
add_subdirectory(src/graphics)
add_subdirectory(src/physics)
add_subdirectory(samples)
