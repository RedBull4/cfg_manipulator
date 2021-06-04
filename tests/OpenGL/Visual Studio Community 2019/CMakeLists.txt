cmake_minimum_required(VERSION 3.15)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
    OpenGL 
    main.cpp 
    cfg_manipulator.cpp
    glad.c
    shaderLoader.cpp
)

target_link_libraries(
    OpenGL PUBLIC
    C:/glfw-3.3.4/build/src/Debug/glfw3.lib
)

target_include_directories(
    OpenGL PUBLIC
    C:/glad/include
    C:/glfw-3.3.4/include
    C:/glm/include
)