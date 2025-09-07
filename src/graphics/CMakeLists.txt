project(graphics LANGUAGES CXX)

set(GRAPHICS_SOURCE_FILES
    de_camera.cpp
    de_reference.cpp
    de_renderSystem.cpp
    de_skybox.cpp)

add_library(graphics STATIC ${GRAPHICS_SOURCE_FILES})
target_include_directories(graphics PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(graphics PUBLIC glad imgui physics ${OPENGL_LIBRARIES})
