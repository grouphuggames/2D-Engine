cmake_minimum_required(VERSION 3.7)
project(2DEngine VERSION 0.1.0)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_compile_options("/std:c++17")
add_executable(Flappy ${headers} ${sources})

target_include_directories(Flappy PUBLIC ${CMAKE_SOURCE_DIR}/glfw-3.3.4.bin.WIN64/include ${CMAKE_SOURCE_DIR}/glew-2.1.0/include)
target_link_libraries(Flappy OpenGL::GL ${CMAKE_SOURCE_DIR}/glfw-3.3.4.bin.WIN64/lib-vc2019/glfw3.lib ${CMAKE_SOURCE_DIR}/glew-2.1.0/lib/Release/x64/glew32s.lib)

#add_custom_command(TARGET Flappy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/FMOD\ SoundSystem/FMOD\ Studio\ API\ Windows/api/core/lib/x64/fmod.dll $<TARGET_FILE_DIR:Flappy>)
