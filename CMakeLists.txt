cmake_minimum_required(VERSION 3.7)
project(ProjectMars VERSION 0.1.0)

add_compile_options("-std=c++17")
add_compile_options("-O2")
add_compile_options("-ffast-math")
add_compile_options("-lstdc++fs")

add_subdirectory(${CMAKE_SOURCE_DIR}/glfw-3.3.4)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_executable(Engine ${headers} ${sources})

target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/glfw-3.3.4/include ${CMAKE_SOURCE_DIR}/glew-2.1.0/include)
target_link_libraries(Engine glfw OpenGL::GL ${CMAKE_SOURCE_DIR}/glew-2.1.0/lib/libGLEW.a)
