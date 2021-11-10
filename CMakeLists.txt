cmake_minimum_required(VERSION 3.7)
project(2DEngine VERSION 0.1.0)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_compile_options("/std:c++17")
add_executable(Engine ${headers} ${sources})

target_link_libraries(Engine OpenGL::GL ${CMAKE_SOURCE_DIR}/lib/glfw3.lib ${CMAKE_SOURCE_DIR}/lib/glew32s.lib ${CMAKE_SOURCE_DIR}/lib/fmod_vc.lib)

target_compile_definitions(Engine PUBLIC _CRT_SECURE_NO_WARNINGS)
add_custom_command(TARGET Engine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lib/fmod.dll $<TARGET_FILE_DIR:Engine>)