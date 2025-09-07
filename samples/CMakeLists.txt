project(src LANGUAGES CXX)

set(MAIN_ENTRY_POINT
    cubeprim_test.hpp
    cubeprim_test.cpp
    main.cpp)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
    GIT PROGRESS TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Populate(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.3.zip
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/build/imgui
)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/build/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui.h
    
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui PUBLIC glfw glad)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
target_compile_definitions(imgui PRIVATE IMGUI_STB_NAMESPACE=imgui_stb)
set_target_properties(imgui PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_executable(samples ${MAIN_ENTRY_POINT})
target_include_directories(samples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(samples PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(samples PUBLIC glfw graphics imgui glad physics ${OPENGL_LIBRARIES})
