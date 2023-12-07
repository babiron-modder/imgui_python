cmake_minimum_required(VERSION 3.2)

project (imPython)
set(CMAKE_CXX_FLAGS "-std=c++11 -DUNICODE -D_UNICODE -pthread -static -lstdc++")

# 変数の設定
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src CACHE STRING "source folder")
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE STRING "include folder")
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib CACHE STRING "lib folder")
set(LIB_GL3W_DIR ${LIB_DIR}/gl3w CACHE STRING "gl3w")
set(LIB_GLFW_DIR ${LIB_DIR}/glfw CACHE STRING "glfw")
set(LIB_IMGUI_DIR ${LIB_DIR}/imgui CACHE STRING "imgui")
set(LIB_PYTHON_DIR ${LIB_DIR}/imgui CACHE STRING "python")

include_directories(
    ${INCLUDE_DIR}
    ${LIB_GL3W_DIR}/include
    ${LIB_GLFW_DIR}/include
    ${LIB_IMGUI_DIR}/include
    ${LIB_PYTHON_DIR}/include
)
link_directories(
    ${LIB_GL3W_DIR}/lib
    ${LIB_GLFW_DIR}/lib
    ${LIB_IMGUI_DIR}/lib
    ${LIB_PYTHON_DIR}/libs
)
add_executable(
    imPython
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/util.cpp
    ${SRC_DIR}/impy.cpp
    ${LIB_GL3W_DIR}/src/gl3w.c
    ${LIB_IMGUI_DIR}/src/imgui.cpp
    ${LIB_IMGUI_DIR}/src/imgui_demo.cpp
    ${LIB_IMGUI_DIR}/src/imgui_draw.cpp
    ${LIB_IMGUI_DIR}/src/imgui_tables.cpp
    ${LIB_IMGUI_DIR}/src/imgui_widgets.cpp
    ${LIB_IMGUI_DIR}/src/imgui_impl_glfw.cpp
    ${LIB_IMGUI_DIR}/src/imgui_impl_opengl3.cpp
)

# 静的・動的ライブラリの設定
# glfwのリンクの設定
target_link_libraries(imPython PUBLIC libglfw3dll.a)
target_link_libraries(imPython PUBLIC glfw3.dll)
target_link_libraries(imPython PUBLIC python312.dll)

