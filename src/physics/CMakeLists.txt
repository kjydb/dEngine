project(physics LANGUAGES CXX)

set(PHYSICS_SOURCE_FILES
    de_cloth.cpp
    de_geometry.cpp
    de_particle.cpp
    de_physicsSystem.cpp
    de_rigidbodyVolume.cpp)

add_library(physics STATIC ${PHYSICS_SOURCE_FILES})
target_include_directories(physics PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(physics PUBLIC glad imgui graphics ${OPENGL_LIBRARIES})
